cmake_minimum_required(VERSION 3.0)
project(GameOfLife CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
if (EMSCRIPTEN)
    set(PLATFORM Web CACHE BOOL "" FORCE)
endif()
#set(GRAPHICS GRAPHICS_API_OPENGL_11 CACHE BOOL "" FORCE) # use opengl 1.1

add_subdirectory(libs/raylib)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --shell-file ../template.html")
    # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()

#add_compile_options(-Wall -Wextra -pedantic)
file(GLOB_RECURSE APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)
add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCES})

# Set version number
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC VERSION="1.0.0")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_NAME="${CMAKE_PROJECT_NAME}")

if (EMSCRIPTEN)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE raylib)
