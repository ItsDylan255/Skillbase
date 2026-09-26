cmake_minimum_required(VERSION 3.19)
project(Skillbase LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/resources)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

if(WIN32)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/skillbase_icon.rc")
endif()

qt_add_executable(Skillbase
    WIN32 MACOSX_BUNDLE
    ${APP_ICON_RESOURCE}

    src/main.cpp

    src/mainwindow.cpp
    include/mainwindow.h
    resources/mainwindow.ui

    src/database.cpp
    include/database.h

    include/hobby.h
    src/hobbyrepository.cpp
    include/hobbyrepository.h

    include/exercise.h
    src/exerciserepository.cpp
    include/exerciserepository.h

    include/routine.h
    include/routinestep.h
    include/routinelog.h
    src/routinerepository.cpp
    include/routinerepository.h

    include/exerciselog.h
    src/exerciselogrepository.cpp
    include/exerciselogrepository.h

    include/goal.h
    src/goalrepository.cpp
    include/goalrepository.h

    src/hobbynoterepository.cpp
    include/hobbynoterepository.h

    include/category.h
    src/categoryrepository.cpp
    include/categoryrepository.h
    resources/category_management_dialog.ui

    include/timelinephase.h
    src/timelinephaserepository.cpp
    include/timelinephaserepository.h

    src/timelinebarwidget.cpp
    include/timelinebarwidget.h

    src/exerciseexecutiondialog.cpp
    include/exerciseexecutiondialog.h
    resources/exercise_execution_dialog.ui

    src/historyentrywidget.cpp
    include/historyentrywidget.h
    resources/historyentrywidget.ui

    src/historydateheaderwidget.cpp
    include/historydateheaderwidget.h
    resources/historydateheaderwidget.ui

    resources/exercise_dialog.ui
    resources/resources.qrc
)

target_include_directories(Skillbase PRIVATE include)

target_link_libraries(Skillbase
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS Skillbase
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Skillbase
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
