FILE (GLOB utf8pp_conversion_HEADERS *.hxx)
FILE (GLOB utf8pp_conversion_SOURCES *.cxx)

INCLUDE_DIRECTORIES (../core)

ADD_LIBRARY (utf8pp_conversion STATIC ${utf8pp_conversion_SOURCES} ${utf8pp_conversion_HEADERS})
INSTALL (TARGETS utf8pp_conversion DESTINATION lib)
INSTALL (FILES ${utf8pp_conversion_HEADERS} DESTINATION include/utf8pp)

