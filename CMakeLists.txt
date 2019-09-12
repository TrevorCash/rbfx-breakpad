cmake_minimum_required (VERSION 3.4.3)

# Set project name
project (rbfx-bugsplat)



add_library(rbfx-bugsplat SHARED bugsplat_rbfx.h bugsplat_rbfx.cpp)

target_compile_definitions(rbfx-bugsplat PUBLIC URHOBUGSPLAT_EXPORTS)

target_include_directories(rbfx-bugsplat PUBLIC . ThirdParty/BugSplat/inc )

find_library(bugsplatlib BugSplat64 ThirdParty/BugSplat/lib64)

target_link_libraries(rbfx-bugsplat PUBLIC Urho3D ${bugsplatlib})

#file(COPY ThirdParty/BugSplat/bin64/BugSplat64.dll
#        DESTINATION ${CMAKE_BINARY_DIR}/bin/RelWithDebInfo
#        DESTINATION ${CMAKE_BINARY_DIR}/bin/Release
#        DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)

add_custom_target(rbfx-bugsplat-dll-copy COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/BugSplat/bin64/BugSplat64.dll ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/BugSplat64.dll)
add_dependencies(rbfx-bugsplat rbfx-bugsplat-dll-copy)