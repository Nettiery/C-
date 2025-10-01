SHELL = /bin/bash

CC = gcc -arch arm64
CFLAGS = -Wall -Werror -Wextra -std=c11
PFLAGS = -Wall -Werror -Wextra -std=c++17
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lcheck -lsubunit -lm
DARWIN_LDFLAGS = -lgtest -lstdc++
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
EXCLUDE_FLAGS1 = --exclude '*/gui/*'
EXCLUDE_FLAGS2 = --exclude '*/test/*'

TEST_DIR=test
COVERAGE_DIR=coverage_report

OS_NAME = $(shell uname)

ifeq ($(OS_NAME),Linux)
OPEN = xdg-open
endif

ifeq ($(OS_NAME),Darwin)
OPEN = open
endif

all: clean, install

install: uninstall clean frontend main.o backend controller.o consol_snake.o consol_tetris.o
	$(CC) $(PFLAGS) main.o frontend.o frontend_tetris.o consol_snake.o consol_tetris.o controller.o backend_snake.a backend_tetris.a backend.o  -lncurses -lstdc++ -o game
	rm -rf *.o *.a 

uninstall:
	rm -rf game 

clean: uninstall
	rm -rf *.o *.a *.gcda *.gcdo *.gcno *.gch test/*.gcdo test/*.gcno test/*.gcda test/unit_test report *.tar *.out report  *.tar

run: clean install
	./game

gcov_report: clear_Highscore clean backend 
	$(CC) $(PFLAGS) --coverage test/unit_test.cpp backend_snake.a $(DARWIN_LDFLAGS) -lncurses -o test/unit_test
	cd test && ./unit_test
	mkdir report
	# gcovr --exclude-unreachable-branches --exclude-throw-branches -r . --html --html-details -o report/report.html
	gcovr --html --html-details -o report/report.html
	$(OPEN) report/report.html

desktop: clean
	rm -rf desk && mkdir desk
	cd gui/desktop/build/ && cmake .. ../../desktop
	cd gui/desktop/build/ && make 
	# cd ./desk/desktop
	./gui/desktop/build/Qt_6_9_0_for_macOS-Release/desktop.app/Contents/MacOS/desktop

dvi:
	@doxygen
	$(OPEN) doxygen/html/index.html

dist: install
	mkdir archiv
	cp -r brick_game gui main.cpp test Makefile archiv
	tar -cf brick_game.tar archiv
	rm -rf archiv

all: install, uninstall, clean, dvi, dist, test, gcov_report

clang_format:
	clang-format -i *.c *.h *.cpp 
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i brick_game/snake/*.cpp brick_game/snake/*.hpp
	clang-format -i brick_game/*.cpp brick_game/*.hpp
	clang-format -i gui/cli/*.cpp gui/cli/*.hpp
	clang-format -i gui/cli/tetris/*.c gui/cli/tetris/*.h
	clang-format -i test/*.c test/*.cpp
	rm -f .clang-format
	
leaks: clear_Highscore clean backend frontend controller.o consol_snake.o
ifeq ($(OS_NAME), Darwin)
	$(CC) $(PFLAGS) test/unit_test.cpp backend_snake.a  consol_snake.o controller.o frontend_tetris.o $(DARWIN_LDFLAGS) -lncurses -o test/unit_test
else
	$(CC) $(CFLAGS) est/unit_test.cpp backend_snake.a  consol_snake.o controller.o frontend_tetris.o  $(LDFLAGS) -lncurses -o test/unit_test
endif
	leaks -atExit -- ./test/unit_test


test: clear_Highscore clean backend frontend controller.o consol_snake.o 
ifeq ($(OS_NAME), Darwin)
	$(CC) $(PFLAGS) test/unit_test.cpp backend_snake.a consol_snake.o controller.o frontend_tetris.o $(DARWIN_LDFLAGS) -lncurses -o test/unit_test
	rm -rf *.o *.a
else
	$(CC) $(CFLAGS) test/unit_test.c backend_tetris.a consol_snake.o controller.o frontend_tetris.o $(LDFLAGS)  -lncurses -o test/unit_test
endif
	cd test && ./unit_test

clear_Highscore:
	$(CC) -c ./test/clear.c -o ./clear_highScore.o
	$(CC) clear_highScore.o -o ./clear_highScore.out
	./clear_highScore.out
	rm -rf *.o *.out

main.o:
	$(CC) $(PFLAGS) -c main.cpp -o main.o

backend: consol_snake.o
	$(CC) $(CFLAGS) -c ./brick_game/tetris/backend_t.c -o backend_tetris.o
	ar rcs backend_tetris.a backend_tetris.o
	ranlib backend_tetris.a
	$(CC) $(PFLAGS) -c ./brick_game/snake/backend_s.cpp -o backend_snake.o
	ar rcs backend_snake.a backend_snake.o
	ranlib backend_snake.a
	$(CC) $(PFLAGS) -c ./brick_game/backend.cpp -o backend.o
	rm -rf backend_tetris.o backend_snake.o

backend.o: 
	$(CC) $(PFLAGS) -c ./brick_game/tetris/backend.cpp 

controller.o: 
	$(CC) $(CFLAGS) -c controller.c controller.h

frontend:
	$(CC) $(PFLAGS) -c ./gui/cli/frontend.cpp -o frontend.o
	$(CC) $(CFLAGS) -c ./gui/cli/tetris/frontend_t.c -o frontend_tetris.o

snake: clean frontend consol_snake.o backend controller.o consol_snake.o 
	$(CC) $(PFLAGS) consol_snake.o frontend.o frontend_tetris.o controller.o  backend_snake.a backend_tetris.a backend.o  -lncurses -lstdc++ -o snake
	rm -rf *.o *.a 

consol_snake.o:
	$(CC) $(PFLAGS) -c ./brick_game/snake/consol_snake.cpp -o consol_snake.o	

consol_tetris.o:
	$(CC) $(CFLAGS) -c brick_game/tetris/consol_tetris.c -o consol_tetris.o	

