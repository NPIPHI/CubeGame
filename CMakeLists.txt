# CMake entry point
cmake_minimum_required (VERSION 3.15)
project (MineCroft)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# these allow variable changin in the debugger
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")
# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)


include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
		ogl-master
)

set(PROJECT_SOURCE_DIR C:/Users/16182/Documents/CPPStuff/ogl-master/ogl-master)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(MineCroft
		src/main.cpp
		common/shader.cpp
		common/shader.hpp
		common/camera.h
		common/camera.cpp
		common/glfwStandardInit.cpp
		common/glfwStandardInit.h
        src/chunck.cpp src/chunck.h src/AxisCamera.cpp src/AxisCamera.h src/GameMap.cpp src/GameMap.h src/character.cpp src/character.h src/IntVect.cpp src/IntVect.h src/controller.cpp src/controller.h src/geometry.cpp src/geometry.h)

add_executable(Test
		test/test.cpp
		)

target_link_libraries(MineCroft
		${ALL_LIBS}
		)