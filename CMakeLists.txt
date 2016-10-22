cmake_minimum_required (VERSION 2.6)
# Maps to Visual Studio solution file (Tutorial.sln)
# The solution will have all targets (exe, lib, dll) 
# as Visual Studio projects (.vcproj)
project (bgSub)

cmake_policy(SET CMP0012 NEW)	#Set policy CMP0012 to suppress CMAKE warning

#message("bgSub_SOURCE_DIR " ${bgSub_SOURCE_DIR})

SET(CMAKE_MODULE_PATH "${bgSub_SOURCE_DIR}/cmake")	#FindFFMPEG.cmake

#message("CMAKE_MODULE_PATH " ${CMAKE_MODULE_PATH})


if (UNIX)
	SET(CMAKE_FIND_LIBRARY_SUFFIXES .so)	#For shared libs 
endif(UNIX)

find_package(OpenCV REQUIRED )	#Look for opencv

#I'm only interested in release config...
 if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES Release)
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
 endif()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and options. 
# Here it is setting the Visual Studio warning level to 4
if (WIN32)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" /EHa)#/W4  /MT")	#Turn exceptions on
endif(WIN32)
if(UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")	#WxWidgets requires file_offset & UsingDLL & __WXGTK__
endif(UNIX)
# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})
 
# Sub-directories where more CMakeLists.txt exist
add_subdirectory(src)
