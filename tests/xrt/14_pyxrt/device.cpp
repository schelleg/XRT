#include "experimental/xrt_device.h"

#include <pybind11/pybind11.h>
namespace py = pybind11;


int main(){return 0;}
PYBIND11_MODULE(device, m) {
m.doc() = "Pybind11 module xrt_device";
m.def("xclProbe", &xclProbe); 
m.def("xrtDeviceOpen", &xrtDeviceOpen,
py::arg("index"),
"A function with name xrtDeviceOpen");
m.def("xrtDeviceClose", &xrtDeviceClose,
py::arg("dhdl"),
"A function with name xrtDeviceClose");
m.def("xrtDeviceLoadXclbin", &xrtDeviceLoadXclbin,
py::arg("dhdl"),
py::arg("xclbin"),
"A function with name xrtDeviceLoadXclbin");
m.def("xrtDeviceLoadXclbinFile", &xrtDeviceLoadXclbinFile,
py::arg("dhdl"),
py::arg("xclbin_filename"),
"A function with name xrtDeviceLoadXclbinFile");
m.def("xrtDeviceGetXclbinUUID", &xrtDeviceGetXclbinUUID,
py::arg("dhld"),
py::arg("out"),
"A function with name xrtDeviceGetXclbinUUID");

 py::class_<xrt::uuid>(m, "uuid")
.def(py::init<char *>())
.def("get",&xrt::uuid::get)
;
 
py::class_<xrt::device>(m, "device")
.def(py::init<>())
.def(py::init<unsigned int>())
.def(py::init<const xrt::device &>())
  //.def(py::init<xrt::device & &>()) // unclear Types
  //.def("operator=", &xrt::device::operator=) // operator()
  //.def("load_xclbin", &xrt::device::load_xclbin) // unclear Types
  .def("load_xclbin", (xrt::uuid (xrt::device::*)(const std::string&)) (&xrt::device::load_xclbin))
  //.def("load_xclbin", &xrt::device::load_xclbin) // overloaded function
.def("get_xclbin_uuid", &xrt::device::get_xclbin_uuid)
  // .def("get_xclbin_section", &xrt::device::get_xclbin_section) // unclear Tupes
  //.def("operatorxclDeviceHandle", &xrt::device::operatorxclDeviceHandle) // not in class
;



 
}
