/**
 * Copyright (C) 2016-2020 Xilinx, Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

//#include <iostream>
//#include <stdexcept>
//#include <string>
//#include <cstring>

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>


// XRT includes
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
/* python 

import pyxrt

xclbin = '/group/xlabs-co/grahams/XRT/tests/xrt/02_simple/kernel.hw_emu.xclbin'
device = pyxrt.device(0)
uuid = device.load_xclbin(xclbin)
simple = pyxrt.kernel(device, uuid.git(), 'simple')
bo0 = pyxrt.bo(device, data_size, pyxrt.XCL_BO_FLAGS_NONE, simple.group_id(0))
bo1 = pyxrt.bo(device, data_size, pyxrt.XCL_BO_FLAGS_NONE, simple.group_id(1))
run = simple.run(bo0, bo1, 0x10)
run.wait()

bo0.sync(pyxrt.XCL_BO_SYNC_BO_FROM_DEVICE, data_size, 0);

 */

/*
 if (device_index >= xclProbe())
   pass

  auto device = xrt::device(device_index);
  auto uuid = device.load_xclbin(xclbin_fnm);


  auto simple = xrt::kernel(device, uuid.get(), "simple");
  auto bo0 = xrt::bo(device, DATA_SIZE, XCL_BO_FLAGS_NONE, simple.group_id(0));
  auto bo1 = xrt::bo(device, DATA_SIZE, XCL_BO_FLAGS_NONE, simple.group_id(1));

  bo0.sync(XCL_BO_SYNC_BO_TO_DEVICE, DATA_SIZE, 0);
  bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE, DATA_SIZE, 0);

  auto run = simple(bo0, bo1, 0x10);
  run.wait();
  bo0.sync(XCL_BO_SYNC_BO_FROM_DEVICE, DATA_SIZE, 0);

 if (device_index >= xclProbe())
    throw std::runtime_error("Cannot find device index (" + std::to_string(device_index) + ") specified");

  auto device = xrt::device(device_index);
  auto uuid = device.load_xclbin(xclbin_fnm);
  run(device, uuid, verbose);
*/

class uuidgraham
{
public:
  xuid_t g_uuid;

  uuidgraham()
  {
    uuid_clear(g_uuid);
    g_uuid[0] = 'a';
    g_uuid[15] = 'b';
  }

  uuidgraham(const xuid_t val)
  {
    uuid_copy(g_uuid,val);
    g_uuid[1] = 'x';
  }

  std::string
  to_string() const
  {
    char str[40] = {0};
    uuid_unparse_lower(g_uuid,str);
    return str;
  }
  const xuid_t& get() const
  {
    return g_uuid;
  }

};






namespace py = pybind11;

PYBIND11_MODULE(pyxrt, m) {
m.doc() = "Pybind11 module xrt";



/*
 *
 *  
 *
 *
 */

 py::class_<uuidgraham>(m, "uuidgraham")
.def(py::init<>())
.def(py::init([](py::array_t<unsigned char> u){ 
    return new uuidgraham((unsigned char*) u.request().ptr);
  }
    ))
.def("get",
      [](uuidgraham & u){
       py::array_t<unsigned char> result = py::array_t<unsigned char>(16);
       py::buffer_info bufinfo = result.request();
       unsigned char* bufptr = (unsigned char*) bufinfo.ptr;
       memcpy(bufptr,u.get(),16);
       return result;
     }
)
.def("to_string", &uuidgraham::to_string)
;


/*
 *
 * XRT:: Functions
 *
 */
 
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

/*
 *
 * XRT:: UUID (needed since UUID classes passed outside of objects)
 *
 */

 
 py::class_<xrt::uuid>(m, "uuid")
.def(py::init<char *>())
   //.def("get",&xrt::uuid::get)
.def("get",
      [](xrt::uuid & u){
       py::array_t<unsigned char> result = py::array_t<unsigned char>(16);
       py::buffer_info bufinfo = result.request();
       unsigned char* bufptr = (unsigned char*) bufinfo.ptr;
       memcpy(bufptr,u.get(),16);
       return result;
     }
)
.def("to_string", &xrt::uuid::to_string)
;
 /*
.def("get", 
       [](xrt::uuid & u){
       py::array_t<char> result = py::array_t<char>(16);
       py::buffer_info bufinfo = result.request();
       char* bufptr = bufinfo.ptr;
       memcpy(bufptr,u.get(),16);
       return result;
       
     }
     )
 */


/*
 *
 * XRT:: Device 
 *
 */

 
py::class_<xrt::device>(m, "device")
.def(py::init<>())
.def(py::init<unsigned int>())
  //.def(py::init<const xrt::device &>())
  //.def(py::init<xrt::device & &>()) // unclear Types
  //.def("operator=", &xrt::device::operator=) // operator()
  //.def("load_xclbin", &xrt::device::load_xclbin) // unclear Types
  //.def("load_xclbin", (xrt::uuid (xrt::device::*)(const std::string&)) (&xrt::device::load_xclbin))
.def("load_xclbin", 
       [](xrt::device & d, const std::string& xclbin){
	 return d.load_xclbin(xclbin);
       }
     )
  //.def("load_xclbin", &xrt::device::load_xclbin) // overloaded function
.def("get_xclbin_uuid", &xrt::device::get_xclbin_uuid)
  // .def("get_xclbin_section", &xrt::device::get_xclbin_section) // unclear Tupes
  //.def("operatorxclDeviceHandle", &xrt::device::operatorxclDeviceHandle) // not in class
;



/*
 *
 * XRT:: Kernel 
 *
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
  //  .def(py::init<xrt::device,const unsigned char*,const std::string &,bool>(),
  //py::arg(""),
  //py::arg(""),
  //py::arg(""),
  //py::arg("exclusive")=false
  //) 
.def(py::init([](xrt::device d, const py::array_t<unsigned char> u, const std::string & n, bool e){ 
      return new xrt::kernel(d, (const unsigned char*) u.request().ptr, n, e);
    }//,
    //py::arg(""),
    //py::arg(""),
    //py::arg(""),
    //py::arg("exclusive")=false
))
.def("group_id", &xrt::kernel::group_id)
.def("write_register", &xrt::kernel::write_register)
.def("read_register", &xrt::kernel::read_register)
  //.def("get_handle", &xrt::kernel::get_handle) // handle type?
  //.def("operator()", &xrt::kernel::operator()) // operator (), ... args
; 


/*
 *
 * XRT:: BO
 *
 */



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







/*

PYBIND11_MODULE(pyxrt, m) {
    // optional module docstring
    m.doc() = "pybind11 of XRT";
    m.def("xclProbe", &xclProbe);
    
    py::class_<xrt::uuid>(m, "uuid")
      .def(py::init<>())
      .def("get", &xrt::uuid::get);

    
    py::class_<xrt::device>(m, "device")
      .def(py::init<>())
      .def("load_xclbin", (xrt::uuid (xrt::device::*)(const std::string&)) (&xrt::device::load_xclbin));

}
// /group/xlabs-co/grahams/XRTschelleg/src/runtime_src/core/include/experimental/xrt_device.h

*/
