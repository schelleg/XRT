find_package(pybind11 REQUIRED)

set(TESTNAME mainpybind)
pybind11_add_module(${TESTNAME}  main.cpp)
target_link_libraries(${TESTNAME} PRIVATE ${xrt_core_LIBRARY} ${xrt_coreutil_LIBRARY})
target_link_libraries(${TESTNAME} PRIVATE ${uuid_LIBRARY} pthread)

install(TARGETS ${TESTNAME}
  LIBRARY DESTINATION ${INSTALL_DIR}/${TESTNAME})


