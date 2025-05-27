cmake_minimum_required(VERSION 3.16)

project(boost_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fuse-ld=lld -pthread -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# V8 installation paths
set(V8_DIR "${CMAKE_SOURCE_DIR}/deps/v8")
set(V8_LIB_DIR "${V8_DIR}/out.gn/x64.release.sample/obj")
set(V8_INCLUDE_DIR "${V8_DIR}/include")

find_package(Qt6 REQUIRED COMPONENTS Test)
find_package(Boost REQUIRED filesystem system)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
include_directories(${V8_INCLUDE_DIR})
link_directories(${V8_LIB_DIR})
set(PROJECT_SOURCES
        src/bootstrap.cpp
        src/run.cpp
        src/Window.h
        src/Window.cpp
        src/layouts/MyLayout.h
        src/layouts/Button.h
        src/layouts/arrow_container/Arrow.h
        src/layouts/arrow_container/Arrow.cpp
        src/layouts/file_widget/FileWidget.h
        src/layouts/file_widget/FileWidget.cpp
        src/layouts/srcoll_utils/CustomScrollArea.h
        src/layouts/srcoll_utils/CustomScrollArea.cpp
        src/layouts/srcoll_utils/ScrollContainer.h
        src/layouts/srcoll_utils/ScrollContainer.cpp
        src/structure/bottom_layout/BottomLayout.h
        src/structure/mid_layout/MidLayout.h
        src/structure/mid_layout/MidLayout.cpp
        src/structure/top_layout/TopLayout.h
        src/structure/top_layout/TopLayout.cpp
        src/application/Application.h
        src/application/Application.cpp
        src/resources.qrc
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(main
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(main
        ${PROJECT_SOURCES}
    )
endif()
add_subdirectory(src bin)
add_subdirectory(tests)
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(main PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(main PRIVATE Boost::filesystem Boost::system)
target_link_libraries(main PRIVATE v8_monolith v8_libbase v8_libplatform dl)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(main)
endif()