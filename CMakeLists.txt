cmake_minimum_required(VERSION 3.4.1)

project(kek)

message("Generating for system " ${CMAKE_SYSTEM_NAME})
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Werror -Wsign-compare -O3")
if(!MSVC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wsign-compare -O3 -ggdb")
endif()

set(GLFW_BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)

#3rd party files
add_subdirectory(deps/openal-soft)
add_subdirectory(deps/Chipmunk2D)
add_subdirectory(deps/glfw)
add_library(glad STATIC deps/glfw/deps/glad_gl.c)

# includes 
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src)
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps) 
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/stb)
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include)
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/deps)
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/deps/glad)
list(APPEND KEK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/Chipmunk2D/include)
set(KEK_INCLUDE_DIRS ${KEK_INCLUDE} PARENT_SCOPE)

include_directories(${KEK_INCLUDE})

#source files
file(GLOB KEK_SOURCE src/kek*)
file(GLOB PAL_SOURCE src/pal*)
file(GLOB KUT_SOURCE src/kut*)

#pal libraies
set(PAL_LIBRARIES glad glfw chipmunk_static OpenAL)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PAL_LIBRARIES ${PAL_LIBRARIES} dl m pthread X11 GL glfw glad)
endif()


#library declartions
add_library(kek_lib STATIC ${KEK_SOURCE} ${PAL_SOURCE} ${KUT_SOURCE})

#linking
target_link_libraries(kek_lib ${PAL_LIBRARIES})

add_compile_definitions(PAL_USE_GLFW)
add_compile_definitions(PAL_USE_OPENGL)


