FILE (GLOB utf8pp_test_HEADERS *.hxx)
FILE (GLOB utf8pp_test_SOURCES *.cxx)

INCLUDE_DIRECTORIES (../src/)

ADD_EXECUTABLE (utf8pp_test ${utf8pp_test_SOURCES} ${utf8pp_test_HEADERS})
TARGET_LINK_LIBRARIES (utf8pp_test utf8pp_core utf8pp_conversion)

