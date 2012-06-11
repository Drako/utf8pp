FILE (GLOB utf8pp_core_HEADERS *.hxx)
FILE (GLOB utf8pp_core_SOURCES *.cxx)

ADD_LIBRARY (utf8pp_core STATIC ${utf8pp_core_SOURCES} ${utf8pp_core_HEADERS})
INSTALL (TARGETS utf8pp_core DESTINATION lib)
INSTALL (FILES ${utf8pp_core_HEADERS} DESTINATION include/utf8pp)
