PROJECT (utf8pp)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.10)

# for g++ and clang++
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7.0")
	   ADD_DEFINITIONS (-std=c++0x)
    ELSE ()
        ADD_DEFINITIONS (-std=c++11)
    ENDIF ()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # will have to adjust this... older clang versions may need -std=c++0x
    # but I could not find an exact version number :-/
    ADD_DEFINITIONS (-std=c++11)
ENDIF ()

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (test)
