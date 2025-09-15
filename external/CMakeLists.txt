set(EXTERNAL_DIR "External")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(NOT TARGET Catch2)
    add_subdirectory(Catch2)

    option(CATCH_INSTALL_DOCS "" OFF)
    option(CATCH_INSTALL_EXTRAS "" OFF)

    set_target_properties(Catch2 PROPERTIES FOLDER ${EXTERNAL_DIR}/Catch2)
endif()
