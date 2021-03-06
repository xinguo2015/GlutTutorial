CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Tutorial)
# SDL directories
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	MESSAGE(STATUS "current platform: Linux ")  
	find_package (OpenGL REQUIRED) 
	find_package (GLUT REQUIRED)
	find_package (GLEW REQUIRED)
	include_directories (${OPENGL_INCLUDE_DIR}) 
	include_directories (${GLUT_INCLUDE_DIR})
	include_directories (${GLEW_INCLUDE_DIR})
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	MESSAGE(STATUS "current platform: Windows")  
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
	MESSAGE(STATUS "current platform: FreeBSD")  
ELSE ()  
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
# cxx flags
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g")
SET(CMAKE_BUILD_TYPE Release) 
SET(CMAKE_BUILD_TYPE Debug) 
# build target
AUX_SOURCE_DIRECTORY(src srcFiles)
GET_FILENAME_COMPONENT(exeName "${CMAKE_SOURCE_DIR}" NAME)
ADD_EXECUTABLE(${exeName} ${srcFiles})
TARGET_LINK_LIBRARIES(${exeName} 
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARY}
	-lm -pthread -lXi -lX11)

