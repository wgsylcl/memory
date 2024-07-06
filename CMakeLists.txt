cmake_minimum_required(VERSION 3.20)

project(memory VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

if(APPLE)
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/MacOS)
    set(CMAKE_MACOSX_DEPLOYMENT_TARGET "10.13")
else()
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()


include_directories(./src)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Svg Multimedia Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Svg Multimedia Network)

find_program(QT_LUPDATE NAMES lupdate)
find_program(QT_LRELEASE NAMES lrelease)
file(GLOB TS_FILE_PATHS ${CMAKE_CURRENT_LIST_DIR}/ *.ts)
add_custom_target(Script-UpdateTranslations
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_DIR_PATH}/i18n
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}_en_US.qm ${PROJECT_NAME}_zh_CN.qm ${APPLICATION_DIR_PATH}/i18n
    SOURCES ${TS_FILE_PATHS}
)

if(WIN32)
    set(3RDPARTY_PATH ${CMAKE_SOURCE_DIR}/3rdparty/win32)
elseif(APPLE)
    set(3RDPARTY_PATH ${CMAKE_SOURCE_DIR}/3rdparty/macos)
endif()

link_directories(${3RDPARTY_PATH}/ffmpeg/lib)
include_directories(${3RDPARTY_PATH}/ffmpeg/include)

qt_add_big_resources(QRC_RESOURCES qml.qrc)
list(APPEND QRC_RESOURCES qml.qrc)
set_property(SOURCE qml.qrc PROPERTY SKIP_AUTORCC ON)

add_definitions(-DPASSWORD="$ENV{PASSWORD}")
add_definitions(-DVERSION="1.0.0")
add_definitions(-DINFOURL="$ENV{memorydata_info_url}")

set(PROJECT_SOURCES
    ${QRC_RESOURCES}
    main.cpp
    src/timelinereader.h src/timelinereader.cpp
    src/studentreader.h src/studentreader.cpp
    src/teacherfilereader.h src/teacherfilereader.cpp
    src/maintool.h src/maintool.cpp
    src/activityhelper.h src/activityhelper.cpp
    src/FluentPlayer.h src/FluentPlayer.cpp
    src/stdafx.h
    src/imageprovider.h src/imageprovider.cpp
    src/base.h src/base.cpp
    src/encodethread.h src/encodethread.cpp
    src/direncoder.h src/direncoder.cpp
    src/direncodemanager.h src/direncodemanager.cpp
    src/database.h src/database.cpp
    src/memoryapplication.h src/memoryapplication.cpp
    src/databaseinitializer.h src/databaseinitializer.cpp
    src/downloader.h src/downloader.cpp
    src/downloadmanager.h src/downloadmanager.cpp
    src/datamanagerhelper.h src/datamanagerhelper.cpp
    src/filelistreader.h src/filelistreader.cpp
    src/profilepictureupdater.h src/profilepictureupdater.cpp
    src/bigfiledownloader.h src/bigfiledownloader.cpp
    src/activityupdater.h src/activityupdater.cpp
    src/uploader.h src/uploader.cpp
    src/packer.h src/packer.cpp
    src/uploadpreviewimageprovider.h src/uploadpreviewimageprovider.cpp
    src/app_dump.h
    src/dataupdatehelper.h src/dataupdatehelper.cpp
)

if(WIN32)
    list(APPEND PROJECT_SOURCES win32_logo.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    QT_MESSAGELOGCONTEXT
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Core_PRIVATE_INCLUDE_DIRS})

add_dependencies(${PROJECT_NAME} Script-UpdateTranslations)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Svg
    avformat
    avcodec
    avutil
    swscale
    swresample
    avdevice
    avfilter
)

if(WIN32)
    file(GLOB_RECURSE DLL_FILES ${3RDPARTY_PATH}/ffmpeg/bin/*.dll ${3RDPARTY_PATH}/ffmpeg/bin/*.exe)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DLL_FILES}
        ${APPLICATION_DIR_PATH}
    )
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()

    if (APPLE)
        find_program(QT_DEPLOY_QT NAMES macdeployqt)
        add_custom_target(Script-DeployRelease
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.app -qmldir=${CMAKE_CURRENT_LIST_DIR}/qml -dmg
            COMMENT "MacOs Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
    if(WIN32)
        find_program(QT_DEPLOY_QT NAMES windeployqt)
        add_custom_target(Script-DeployRelease
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_SOURCE_DIR}/dist/data
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.exe -qmldir=${CMAKE_CURRENT_LIST_DIR}/qml
            COMMENT "Windows Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
    if (APPLE)
        find_program(QT_DEPLOY_QT NAMES macdeployqt)
        add_custom_target(Script-DeployDebug
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}.app -qmldir=${CMAKE_CURRENT_LIST_DIR} -dmg
            COMMENT "MacOS Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
    if(WIN32)
        find_program(QT_DEPLOY_QT NAMES windeployqt)
        add_custom_target(Script-DeployDebug
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}.exe -qmldir=${CMAKE_CURRENT_LIST_DIR}/qml
            COMMENT "Windows Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

add_custom_target(RELOAD_DATABASE
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APPLICATION_DIR_PATH}/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${APPLICATION_DIR_PATH}/data
    )
