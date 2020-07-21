#include "experimental/xrt_bo.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;



/*
class kerneltest
{
public:
  XCL_DRIVER_DLLESPEC
  kerneltest(xclDeviceHandle dhdl, const std::string& xclbin_id, const std::string& name, bool exclusive=false){
  }

  XCL_DRIVER_DLLESPEC
  kerneltest(xclDeviceHandle dhdl, const xuid_t xclbin_id, const std::string& name, bool exclusive=false){
  }
  
};
*/


/* 
py::class_<kerneltest>(m, "kerneltest")
.def(py::init<xclDeviceHandle,const std::string &,const std::string &,bool>())
.def(py::init<xclDeviceHandle,char*,const std::string &,bool>())
;
*/



int main(){return 0;}
PYBIND11_MODULE(bo, m) {
m.doc() = "Pybind11 module xrt_bo";
 
m.def("xrtBOAllocUserPtr", &xrtBOAllocUserPtr,
py::arg("dhdl"),
py::arg("userptr"),
py::arg("size"),
py::arg("flags"),
py::arg("grp"),
"A function with name xrtBOAllocUserPtr");

 m.def("xrtBOAlloc", &xrtBOAlloc,
py::arg("dhdl"),
py::arg("size"),
py::arg("flags"),
py::arg("grp"),
"A function with name xrtBOAlloc");

 m.def("xrtBOSubAlloc", &xrtBOSubAlloc,
py::arg("parent"),
py::arg("size"),
py::arg("offset"),
"A function with name xrtBOSubAlloc");

 m.def("xrtBOFree", &xrtBOFree,
py::arg("handle"),
"A function with name xrtBOFree");
 
 m.def("xrtBOSync", &xrtBOSync,
py::arg("handle"),
py::arg("dir"),
py::arg("size"),
py::arg("offset"),
"A function with name xrtBOSync");

 m.def("xrtBOMap", &xrtBOMap,
py::arg("handle"),
"A function with name xrtBOMap");

 m.def("xrtBOWrite", &xrtBOWrite,
py::arg("handle"),
py::arg("src"),
py::arg("size"),
py::arg("seek"),
"A function with name xrtBOWrite");

 m.def("xrtBORead", &xrtBORead,
py::arg("handle"),
py::arg("dst"),
py::arg("size"),
py::arg("skip"),
"A function with name xrtBORead");

 // m.attr("buffer_flags") = py::cast(xrt::buffer_flags); // do I need these?
 // m.attr("memory_group") = py::cast(xrt::memory_group); // do I need these?

 
 py::class_<xrt::bo>(m, "bo")
.def(py::init<>())
   .def(py::init<xclDeviceHandle,void *,size_t,xrt::buffer_flags,xrt::memory_group>())
.def(py::init<xclDeviceHandle,size_t,xrt::buffer_flags,xrt::memory_group>())
.def(py::init<const xrt::bo &,size_t,size_t>())
.def(py::init<const xrt::bo &>())
   //.def(py::init<xrt::bo & &>())
   // .def("operator=", &xrt::bo::operator=)
.def("sync", &xrt::bo::sync)
   //.def("map", &xrt::bo::map) // void/MapType versions
   //.def("map", &xrt::bo::map) // void/MapType versions
.def("write", &xrt::bo::write)
.def("read", &xrt::bo::read)
   //.def("get_handle", &xrt::bo::get_handle)
;
/*
 */

}
