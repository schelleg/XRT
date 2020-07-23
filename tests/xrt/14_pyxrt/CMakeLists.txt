find_package(pybind11 REQUIRED)
set(DESTNAME pyxrt)


# list(APPEND HEADERS device kernel bo)
list(APPEND HEADERS pyxrt)
foreach(HEADER IN LISTS HEADERS)

  pybind11_add_module(${HEADER}  ${HEADER}.cpp)
  target_link_libraries(${HEADER} PRIVATE ${xrt_core_LIBRARY} ${xrt_coreutil_LIBRARY})

  if (NOT WIN32)
    target_link_libraries(${HEADER} PRIVATE ${uuid_LIBRARY} pthread)
  endif(NOT WIN32)

  install(TARGETS ${HEADER}
    LIBRARY DESTINATION ${INSTALL_DIR}/${DESTNAME})

endforeach()


