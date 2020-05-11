cmake_minimum_required(VERSION 3.10)
project(parrlib)

#include_directories(include)

#file(GLOB SOURCES "src/*.cpp")

file(GLOB_RECURSE SOURCE_FILES
                    ${CMAKE_SOURCE_DIR}/src/*.c
                    ${CMAKE_SOURCE_DIR}/src/**/*.c
                    ${CMAKE_SOURCE_DIR}/src/*.cpp
                    ${CMAKE_SOURCE_DIR}/src/**/*.cpp)

file(GLOB_RECURSE HEADER_FILES
                    ${CMAKE_SOURCE_DIR}/src/*.h
                    ${CMAKE_SOURCE_DIR}/src/**/*.h
                    ${CMAKE_SOURCE_DIR}/src/*.hpp
                    ${CMAKE_SOURCE_DIR}/src/**/*.hpp
					
					${CMAKE_SOURCE_DIR}/include/*.h
					${CMAKE_SOURCE_DIR}/include/**/*.h
                    ${CMAKE_SOURCE_DIR}/include/*.hpp
                    ${CMAKE_SOURCE_DIR}/include/**/*.hpp
					
					${CMAKE_SOURCE_DIR}/thirdparty/include/*.h
					${CMAKE_SOURCE_DIR}/thirdparty/include/**/*.h
					${CMAKE_SOURCE_DIR}/thirdparty/include/*.hpp
					${CMAKE_SOURCE_DIR}/thirdparty/include/**/*.hpp)

# Scan through resource folder for updated files and copy if none existing or changed
file (GLOB_RECURSE resources "resources/*.*")
foreach(resource ${resources})
    get_filename_component(filename ${resource} NAME)
    get_filename_component(dir ${resource} DIRECTORY)
    get_filename_component(dirname ${dir} NAME)

    set (output "")

    while(NOT ${dirname} STREQUAL resources)
        get_filename_component(path_component ${dir} NAME)
        set (output "${path_component}/${output}")
        get_filename_component(dir ${dir} DIRECTORY)
        get_filename_component(dirname ${dir} NAME)
    endwhile()

    set(output "${CMAKE_CURRENT_BINARY_DIR}/resources/${output}/${filename}")

    add_custom_command(
        COMMENT "Moving updated resource-file '${filename}'"
        OUTPUT ${output}
        DEPENDS ${resource}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${resource}
        ${output}
    )
    add_custom_target(${filename} ALL DEPENDS ${resource} ${output})
endforeach()

#SET(BUILD_SHARED_LIBS OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")

#link_libraries("-static")

add_library(parrlib STATIC ${SOURCE_FILES})

#add_executable(mytest ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(parrlib -static)

#target_compile_options(mytest PUBLIC/PRIVATE /MT)
#target_link_options(mytest PUBLIC/PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found")

#add_library(GLAD "src/glad.c")
#add_library(GLFW STATIC "${CMAKE_SOURCE_DIR}/thirdparty/lib/glfw3")
#add_library(SOIL STATIC "${CMAKE_SOURCE_DIR}/thirdparty/lib/SOIL")
#add_library(FREETYPE2 STATIC "${CMAKE_SOURCE_DIR}/thirdparty/lib/freetype")
#add_library(ASSIMP STATIC "${CMAKE_SOURCE_DIR}/thirdparty/lib/assimp-vc142-mt")
#add_library(ALUT STATIC "${CMAKE_SOURCE_DIR}/thirdparty/lib/alut")

include_directories("${CMAKE_SOURCE_DIR}/src"  "${CMAKE_SOURCE_DIR}/include"  "${CMAKE_SOURCE_DIR}/thirdparty/include")

target_include_directories(parrlib PRIVATE ${OPENGL_INCLUDE_DIR})
#target_link_libraries(mytest  
#							"${CMAKE_SOURCE_DIR}/thirdparty/lib/glfw3"
#							"${CMAKE_SOURCE_DIR}/thirdparty/lib/SOIL"
#							"${CMAKE_SOURCE_DIR}/thirdparty/lib/freetype"
#							"${CMAKE_SOURCE_DIR}/thirdparty/lib/assimp-vc142-mt"
#							"${CMAKE_SOURCE_DIR}/thirdparty/lib/alut"
#							${OPENGL_gl_LIBRARY})

