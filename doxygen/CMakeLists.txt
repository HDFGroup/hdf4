cmake_minimum_required (VERSION 3.18)
project (HDF4_DOXYGEN C)

#-----------------------------------------------------------------------------
# Option to build documentation
#-----------------------------------------------------------------------------
if (DOXYGEN_FOUND)
  set (DOXYGEN_PACKAGE ${HDF4_PACKAGE_NAME})
  set (DOXYGEN_VERSION_STRING ${HDF4_PACKAGE_VERSION_STRING})
  set (DOXYGEN_DIR ${HDF4_DOXYGEN_DIR})
  set (DOXYGEN_INCLUDE_ALIASES_PATH ${HDF4_DOXYGEN_DIR})
  set (DOXYGEN_INCLUDE_ALIASES aliases)
  set (DOXYGEN_VERBATIM_VARS DOXYGEN_INCLUDE_ALIASES)
  set (DOXYGEN_PROJECT_LOGO ${HDF4_DOXYGEN_DIR}/img/HDFG-logo.png)
  set (DOXYGEN_PROJECT_BRIEF "API Reference")
  set (DOXYGEN_INPUT_DIRECTORY "${HDF4_SOURCE_DIR} ${HDF4_DOXYGEN_DIR} ${HDF4_DOXYGEN_DIR}/dox ${HDF4_GENERATED_SOURCE_DIR}")
  set (DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set (DOXYGEN_MACRO_EXPANSION YES)
  set (DOXYGEN_OUTPUT_DIRECTORY ${HDF4_BINARY_DIR}/hdf4lib_docs)
  set (DOXYGEN_EXAMPLES_DIRECTORY "${HDF4_DOXYGEN_DIR}/dox")
  set (DOXYGEN_LAYOUT_FILE ${HDF4_DOXYGEN_DIR}/hdf4doxy_layout.xml)
  set (DOXYGEN_HTML_HEADER ${HDF4_DOXYGEN_DIR}/hdf4_header.html)
  set (DOXYGEN_HTML_FOOTER ${HDF4_DOXYGEN_DIR}/hdf4_footer.html)
  set (DOXYGEN_HTML_EXTRA_STYLESHEET ${HDF4_DOXYGEN_DIR}/hdf4doxy.css)
  set (DOXYGEN_HTML_EXTRA_FILES "${HDF4_DOXYGEN_DIR}/hdf4_navtree_hacks.js")
  set (DOXYGEN_TAG_FILE ${HDF4_BINARY_DIR}/hdf4.tag)
  set (DOXYGEN_SERVER_BASED_SEARCH NO)
  set (DOXYGEN_EXTERNAL_SEARCH NO)
  set (DOXYGEN_SEARCHENGINE_URL)
  set (DOXYGEN_STRIP_FROM_PATH ${HDF4_SOURCE_DIR})
  set (DOXYGEN_STRIP_FROM_INC_PATH ${HDF4_SOURCE_DIR})
  set (DOXYGEN_PREDEFINED "H4_DOXYGEN")
  set (DOXYGEN_WARN_AS_ERROR ${HDF4_DOXY_WARNINGS})

# This configure and individual custom targets work together
  # Replace variables inside @@ with the current values
  configure_file (${HDF4_DOXYGEN_DIR}/Doxyfile.in ${HDF4_BINARY_DIR}/Doxyfile @ONLY)

  install (
      DIRECTORY ${HDF4_BINARY_DIR}/hdf4lib_docs/html
      DESTINATION ${HDF4_INSTALL_DOC_DIR}
      COMPONENT hdfdocuments
  )
  
  add_custom_target (hdf4_doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${HDF4_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${HDF4_DOXYGEN_DIR}
        COMMENT "Generating HDF4 documentation with Doxygen"
        VERBATIM )

  if (NOT TARGET doxygen)
    add_custom_target (doxygen)
  endif ()

  add_dependencies (doxygen hdf4_doc)

endif ()
