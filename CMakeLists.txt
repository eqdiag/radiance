# CMakeList.txt : CMake project for learnVk, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("learnVk")

## get submodules from github
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
## Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

##           CHECK ALL THE SUBMODULES
##if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
#   message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()




file(GLOB src
    "src/math/*.cpp"
    "external/tiny_obj_loader/tiny_obj_loader.cpp"
    "external/stb_img/stb_image.cpp"
)


#Set asset paths
set(ASSET_DIR 
	" \"${CMAKE_CURRENT_LIST_DIR}/assets/\" "
)

set(MODEL_DIR 
    " \"${CMAKE_CURRENT_LIST_DIR}/assets/models/\" "
)

set(TEXTURE_DIR 
    " \"${CMAKE_CURRENT_LIST_DIR}/assets/textures/\" "
)

message("Using Asset directory: ${ASSET_DIR}")
message("Using Model directory: ${MODEL_DIR}")
message("Using Image directory: ${TEXTURE_DIR}")



configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/src/settings.h.in"
    "${CMAKE_CURRENT_LIST_DIR}/src/settings.h"
)


#add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})
add_executable (
    radiance
    src/main.cpp
    ${src}
)


target_include_directories(
    radiance
    PUBLIC external/stb_img
    PUBLIC external/tiny_obj
    PUBLIC src
)

set_property(TARGET radiance PROPERTY CXX_STANDARD 17)





#TODO: Build shared libs for debugging?, but not release
OPTION( BUILD_SHARED_LIBS
  "Do not build package with shared libraries."
  OFF
)


