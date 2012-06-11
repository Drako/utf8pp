PROJECT (utf8pp)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# MSVC doesn't support enough C++11
IF (MSVC)
	MESSAGE (FATAL_ERROR "Visual C++ does not support enough C++11 to compile this library.")
ENDIF ()

# for g++ and clang++
ADD_DEFINITIONS (-std=c++0x)
	
ADD_SUBDIRECTORY (src)
