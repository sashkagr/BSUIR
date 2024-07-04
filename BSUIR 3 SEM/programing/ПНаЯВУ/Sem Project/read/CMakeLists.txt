cmake_minimum_required(VERSION 3.5)

project(reading VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR}Sql REQUIRED)


include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})
include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include") # Replace with the actual path to the MySQL C++ Connector includes

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(reading
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        fb2helper.h fb2helper.cpp
        database.h database.cpp
        book.h book.cpp

        reading.h reading.cpp reading.ui
        uihelper.h uihelper.cpp
        baseentity.h baseentity.cpp
        shelf.h shelf.cpp
        shelveslist.h shelveslist.cpp shelveslist.ui
        addshelfdialog.h addshelfdialog.cpp addshelfdialog.ui
        shelfbooks.h shelfbooks.cpp shelfbooks.ui
        selectbookstoshelf.h selectbookstoshelf.cpp selectbookstoshelf.ui
        customapplication.h customapplication.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET reading APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(reading SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(reading
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(reading PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(reading PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(reading PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.dll")
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.reading)
endif()
set_target_properties(reading PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS reading
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(reading)
endif()
