find_package(pybind11 REQUIRED)
set(DESTNAME pyxrt)


pybind11_add_module(pyxrt  pyxrt.cpp)
target_link_libraries(pyxrt PRIVATE ${xrt_core_LIBRARY} ${xrt_coreutil_LIBRARY})

if (NOT WIN32)
  target_link_libraries(pyxrt PRIVATE ${uuid_LIBRARY} pthread)
endif(NOT WIN32)

install(TARGETS pyxrt
  LIBRARY DESTINATION ${INSTALL_DIR}/${DESTNAME})
