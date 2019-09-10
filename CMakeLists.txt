cmake_minimum_required (VERSION 3.4.3)

add_library(rbfx-bugsplat SHARED bugsplat_rbfx.h)

target_compile_definitions(rbfx-bugsplat PUBLIC URHOBUGSPLAT_EXPORTS)

target_include_directories(rbfx-bugsplat PUBLIC ./ThirdParty/BugSplat/inc )

find_library(bugsplatlib BugSplat64 /ThirdParty/BugSplat/lib64)

target_link_libraries(rbfx-bugsplat Urho3D bugsplatlib)


set_target_properties(rbfx-bugsplat PROPERTIES LINKER_LANGUAGE CXX)