cmake_minimum_required(VERSION 3.16)

project(boost_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

# Adjust to your Qt install path
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Test Widgets)
set(PROJECT_SOURCES
        src/bootstrap.cpp
        src/run.cpp
        src/Window.h
        src/Window.cpp
        src/v8_stuff/V8_Exec.h
        src/v8_stuff/V8_Exec.cpp
        src/v8_stuff/activeTabUtils/activeTab.h
        src/v8_stuff/activeTabUtils/activeTab.cpp
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
        src/console_widget/ConsoleWidget.h
        src/console_widget/ConsoleWidget.cpp
        src/application/ActiveTab.h
        src/application/Application.h
        src/application/Application.cpp
        src/resources.qrc
)
add_subdirectory(src bin)
add_subdirectory(tests)
add_executable(main ${PROJECT_SOURCES})

target_link_libraries(main PRIVATE Qt6::Widgets)

set(V8_ROOT "${CMAKE_SOURCE_DIR}/deps/v8")

target_include_directories(main PRIVATE
    "${V8_ROOT}"
    "${V8_ROOT}/include"
    "${V8_ROOT}/out.gn/x64.release.sample/obj"
)

target_compile_definitions(main PRIVATE
    V8_COMPRESS_POINTERS
    V8_ENABLE_SANDBOX
)

target_link_libraries(main PRIVATE
    "${V8_ROOT}/out.gn/x64.release.sample/obj/v8_monolith.lib"
    kernel32.lib
    user32.lib
    advapi32.lib
    dbghelp.lib
    version.lib
    winmm.lib
)