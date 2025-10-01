include("/Users/olga/Desktop/new_version/CPP3_BrickGame_v2.0-1/src/gui/desktop/out/build/Настройка предустановки с помощью файла цепочки инструментов/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/desktop-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE desktop.app
)
