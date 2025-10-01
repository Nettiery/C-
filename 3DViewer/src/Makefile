GPP = g++ -Wall -Werror -Wextra -std=c++20

CC = gcc
# GPP = g++ -std=c++20
PKG_FLAGS = $(shell pkg-config --cflags --libs Qt6Widgets Qt6OpenGLWidgets)
COV_FLAG = --coverage

CLI_FLAG = -DUSE_CLI
DESKTOP_FLAG = -DUSE_DESKTOP

INSTALL_DIR ?= ./install
DIST_DIR ?= ./dist
DOCS_DIR = ./docs
OS = $(shell uname -s)
MOC = /usr/lib/qt6/libexec/moc

# Директории
BUILD_DIR = build
CORE_DIR = .
MODEL_DIR = ./model
GUI_DIR = ./gui
GIF_DIR = ./gifimage
GIFLIB_DIR = ./3rdParty/giflib
TEST_DIR = ./tests
GUI_LIB = $(BUILD_DIR)/gui.a
MODEL_LIB = $(BUILD_DIR)/model.a

# Источники
MODEL_SRC = $(wildcard $(MODEL_DIR)/*.cpp)
GUI_SRC := $(wildcard $(GUI_DIR)/*.cpp)
GIF_SRC := $(wildcard $(GIF_DIR)/*.cpp)
GIFLIB_SRC := $(wildcard $(GIFLIB_DIR)/*.c)
MOC_HEADERS = gui/glwidget.h gui/mainwindow.h
MOC_SOURCES = $(patsubst gui/%.h,build/moc_%.cpp,$(filter gui/%.h,$(MOC_HEADERS)))
MOC_OBJECTS = $(patsubst build/%.cpp,build/%.o,$(MOC_SOURCES))

TEST_SRC = $(wildcard $(TEST_DIR)/*test.cpp)

# Объектные файлы
MODEL_OBJ := $(addprefix $(BUILD_DIR)/, $(MODEL_SRC:$(MODEL_DIR)/%.cpp=%.o))
GUI_OBJ := $(patsubst $(GUI_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(GUI_SRC))
GIF_OBJ := $(patsubst $(GIF_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(GIF_SRC))
GIFLIB_OBJ := $(patsubst $(GIFLIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(GIFLIB_SRC))
GUI_OBJ += $(MOC_OBJECTS)
TEST_OBJ := $(addprefix $(BUILD_DIR)/, $(TEST_SRC:$(TEST_DIR)/%.cpp=%.o))

ifeq (${OS}, Linux)
	TEST_FLAGS = -lgtest -lgtest_main -pthread
	MEM_CHCK = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

else
	TEST_FLAGS = -lcheck
	MEM_CHCK = leaks -atExit --
endif

all: total_clean install dist mem_check dvi

$(BUILD_DIR)/%.o: $(MODEL_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Создаём директорию для объектного файла, если она не существует
	$(GPP) $(PKG_FLAGS) -c $< -o $@
	
# Правило для генерации moc_*.cpp
build/moc_%.cpp: gui/%.h
	@mkdir -p $(dir $@)
	$(MOC) $< -o $@

$(BUILD_DIR)/%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GPP) $(PKG_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(GIF_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GPP) $(PKG_FLAGS) -I$(GIFLIB_DIR) -c $< -o $@

$(BUILD_DIR)/%.o: $(GIFLIB_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GPP) $(PKG_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Создаём директорию для объектного файла, если она не существует
	$(GPP) -c $< -o $@


# Правило для создания статической библиотеки логики
$(MODEL_LIB): $(MODEL_OBJ)
	ar rc $@ $(MODEL_OBJ)

# Правило для создания статической библиотеки интерфейса
$(GUI_LIB): $(GUI_OBJ) $(GIF_OBJ) $(GIFLIB_OBJ)
	ar rc $@ $(GUI_OBJ) $(GIF_OBJ) $(GIFLIB_OBJ)

libs: total_clean $(MODEL_LIB) $(GUI_LIB)
	rm -f $(BUILD_DIR)/glwidget.cpp
	rm -f $(BUILD_DIR)/mainwindow.cpp

# Правило для создания исполняемого файла
viewer: libs
	$(GPP) $(GUI_LIB) $(MODEL_LIB) $(PKG_FLAGS) -lGL -o $(BUILD_DIR)/3d_viewer
# 	unzip tests/tests_files/dallas_city.zip -d tests/tests_files/


# Правило для создания и запуска test файла
test: total_clean $(TEST_OBJ) $(MODEL_OBJ) 
	$(GPP) $(COV_FLAG) $(TEST_OBJ) $(MODEL_SRC) ${TEST_FLAGS} -o $(BUILD_DIR)/test
	rm -f $(BUILD_DIR)/*.o
	$(BUILD_DIR)/test

gcov_report: test
	lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage.info \
		--rc geninfo_unexecuted_blocks=1
	lcov --remove $(BUILD_DIR)/coverage.info '/usr/*' '*/tests/*' '*/ext/*' '*/bits/*' \
	    --output-file $(BUILD_DIR)/test.info \
	    --rc geninfo_unexecuted_blocks=1 \
	    --ignore-errors mismatch,mismatch,unused
	genhtml $(BUILD_DIR)/test.info --output-directory $(BUILD_DIR)/coverage_report \
		--rc geninfo_unexecuted_blocks=1

mem_check: test
	${MEM_CHCK} $(BUILD_DIR)/test ${MEM_CHCK_2}

cpp_check:
	cppcheck --language=c++ model/*.h model/*.cpp controller/*.h gui/*.h gui/*.cpp gifimage/*.h gifimage/*.cpp

install: uninstall viewer
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/lib
	cp $(MODEL_LIB) $(INSTALL_DIR)/lib
	cp $(GUI_LIB) $(INSTALL_DIR)/lib
	cp $(BUILD_DIR)/3d_viewer $(INSTALL_DIR)/bin
	# cp $(BUILD_DIR)/settings.conf $(INSTALL_DIR)/bin

uninstall:
	rm -rf $(INSTALL_DIR)

dist:
	rm -rf $(DIST_DIR)
	mkdir -p $(DIST_DIR)
	cp -r controller $(DIST_DIR)
	cp -r model $(DIST_DIR)
	cp -r gui $(DIST_DIR)
	cp -r gifimage $(DIST_DIR)
	cp -r 3rdParty $(DIST_DIR)
	cp -r tests $(DIST_DIR)
	cp -r Makefile $(DIST_DIR)
	cp -r Doxyfile $(DIST_DIR)

dvi:
	rm -rf $(DOCS_DIR)
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_DIR)
	rm -rf $(DOCS_DIR)

total_clean: clean uninstall
	rm -rf ./settings.conf

clang_format:
	clang-format -i ./controller/*.h -style=Google
	clang-format -i ./model/*.cpp ./model/*.h ./gui/*.cpp ./gui/*.h ./gifimage/*.cpp ./gifimage/*.h ./tests/*.cpp ./tests/*.h -style=Google 

clang_format_test:
	~/llvm-build/bin/clang-format -n ./*/*.cpp ./*/*.h -style=Google 

git_add:
	git add ./model/*.cpp ./model/*.h ./gui/*.cpp ./gui/*.h ./tests/*.cpp *.cpp  ./tests/*.h ../.gitignore Makefile *.obj readme.md
	git add ./controller/*.h Doxyfile ./gifimage/*.cpp ./gifimage/*.h ./tests/tests_files/dallas_city.zip