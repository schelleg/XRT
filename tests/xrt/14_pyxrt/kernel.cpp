#include "experimental/xrt_kernel.h"

//#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
#include <pybind11/operators.h>
namespace py = pybind11;




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




int main(){return 0;}
PYBIND11_MODULE(kernel, m) {
m.doc() = "Pybind11 module xrt_kernel";

/* 
py::class_<kerneltest>(m, "kerneltest")
.def(py::init<xclDeviceHandle,const std::string &,const std::string &,bool>())
.def(py::init<xclDeviceHandle,char*,const std::string &,bool>())
;
*/


 m.def("xrtPLKernelOpen", &xrtPLKernelOpen,
py::arg("deviceHandle"),
py::arg("xclbinId"),
py::arg("name"),
"A function with name xrtPLKernelOpen");


m.def("xrtPLKernelOpenExclusive", &xrtPLKernelOpenExclusive,
py::arg("deviceHandle"),
py::arg("xclbinId"),
py::arg("name"),
"A function with name xrtPLKernelOpenExclusive");

m.def("xrtKernelClose", &xrtKernelClose,
py::arg("kernelHandle"),
"A function with name xrtKernelClose");

m.def("xrtKernelArgGroupId", &xrtKernelArgGroupId,
py::arg("kernelHandle"),
py::arg("argno"),
"A function with name xrtKernelArgGroupId");

m.def("xrtKernelReadRegister", &xrtKernelReadRegister,
py::arg(""),
py::arg("offset"),
py::arg("datap"),
"A function with name xrtKernelReadRegister");

m.def("xrtKernelWriteRegister", &xrtKernelWriteRegister,
py::arg(""),
py::arg("offset"),
py::arg("data"),
"A function with name xrtKernelWriteRegister");
/*
 m.def("xrtKernelRun", &xrtKernelRun, // use the ... for extra args..
py::arg("kernelHandle"),
"A function with name xrtKernelRun");
*/
m.def("xrtRunOpen", &xrtRunOpen,
py::arg("kernelHandle"),
"A function with name xrtRunOpen");

/*
m.def("xrtRunSetArg", &xrtRunSetArg, // use the ... for extra args..
py::arg("runHandle"),
py::arg("index"),
"A function with name xrtRunSetArg");

m.def("xrtRunUpdateArg", &xrtRunUpdateArg, // use the ... for extra args..
py::arg("rhdl"),
py::arg("index"),
"A function with name xrtRunUpdateArg");
*/
m.def("xrtRunStart", &xrtRunStart,
py::arg("runHandle"),
"A function with name xrtRunStart");

m.def("xrtRunWait", &xrtRunWait,
py::arg("runHandle"),
"A function with name xrtRunWait");

m.def("xrtRunWaitFor", &xrtRunWaitFor,
py::arg("runHandle"),
py::arg("timeout_ms"),
"A function with name xrtRunWaitFor");

m.def("xrtRunState", &xrtRunState,
py::arg("runHandle"),
"A function with name xrtRunState");

m.def("xrtRunSetCallback", &xrtRunSetCallback,
py::arg("runHandle"),
py::arg("state"),
py::arg(""),
py::arg("data"),
"A function with name xrtRunSetCallback");

m.def("xrtRunClose", &xrtRunClose,
py::arg("runHandle"),
"A function with name xrtRunClose");




 py::class_<xrt::run>(m, "run")
.def(py::init<>())
.def(py::init<const xrt::kernel &>()) // init errors
.def("start", &xrt::run::start)
.def("wait", &xrt::run::wait)
.def("state", &xrt::run::state)
.def("add_callback", &xrt::run::add_callback)
  //.def("operatorbool", &xrt::run::operatorbool)  // operator bool()
  //.def("set_arg", &xrt::run::set_arg) //overloaded
  //.def("set_arg", &xrt::run::set_arg) //overloaded
  //.def("update_arg", &xrt::run::update_arg) //overloaded
  //.def("update_arg", &xrt::run::update_arg) //overloaded
  //.def("operator()", &xrt::run::operator()) // operator (), ... args
;



py::class_<xrt::kernel>(m, "kernel")
  //.def(py::init<xclDeviceHandle,const std::string &,const std::string &,bool>())
.def(py::init<xclDeviceHandle,const char*,const std::string &,bool>())  
  .def("group_id", &xrt::kernel::group_id)
  .def("write_register", &xrt::kernel::write_register)
  .def("read_register", &xrt::kernel::read_register)
  //.def("get_handle", &xrt::kernel::get_handle) // handle type?
  //.def("operator()", &xrt::kernel::operator()) // operator (), ... args
 ; 


 
 
 
}
