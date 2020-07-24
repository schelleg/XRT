set(TESTNAME "02_simple")

add_executable(02_simple main.cpp)
target_link_libraries(02_simple PRIVATE ${xrt_core_LIBRARY} ${xrt_coreutil_LIBRARY})

if (NOT WIN32)
  target_link_libraries(02_simple PRIVATE ${uuid_LIBRARY} pthread)
endif(NOT WIN32)

install(TARGETS 02_simple
  RUNTIME DESTINATION ${INSTALL_DIR}/${TESTNAME})





# find_package(pybind11 REQUIRED)

# set(TESTNAME mainpybind)
# pybind11_add_module(${TESTNAME}  main.cpp)
# target_link_libraries(${TESTNAME} PRIVATE ${xrt_core_LIBRARY} ${xrt_coreutil_LIBRARY})
# target_link_libraries(${TESTNAME} PRIVATE ${uuid_LIBRARY} pthread)

# install(TARGETS ${TESTNAME}
#   LIBRARY DESTINATION ${INSTALL_DIR}/${TESTNAME})


