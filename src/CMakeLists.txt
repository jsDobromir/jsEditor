# add_library(sut STATIC Window.h Window.cpp run.cpp layouts/MyLayout.h layouts/Button.h structure/bottom_layout/BottomLayout.h structure/top_layout/TopLayout.h structure/mid_layout/MidLayout.h structure/mid_layout/MidLayout.cpp structure/top_layout/TopLayout.cpp 
#             layouts/srcoll_utils/ScrollContainer.h layouts/srcoll_utils/ScrollContainer.cpp layouts/srcoll_utils/CustomScrollArea.h layouts/srcoll_utils/CustomScrollArea.cpp
#             layouts/file_widget/FileWidget.h layouts/file_widget/FileWidget.cpp layouts/arrow_container/Arrow.h layouts/arrow_container/Arrow.cpp application/Application.h application/Application.cpp)
# target_include_directories(sut PUBLIC .)

# # Link Qt to the sut library (QPainter is used in Window.cpp)
# target_link_libraries(sut PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# add_executable(bootstrap bootstrap.cpp)
# target_link_libraries(bootstrap PRIVATE sut)

# target_link_libraries(bootstrap PRIVATE
#     "${V8_ROOT}/out.gn/x64.release.sample/obj/v8_monolith.lib"
#     kernel32.lib
#     user32.lib
#     advapi32.lib
#     dbghelp.lib
#     version.lib
#     winmm.lib
# )