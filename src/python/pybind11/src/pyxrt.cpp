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


#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <tuple>
#include <typeinfo>

// XRT includes
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"
#include "xrt.h"

// Pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

PYBIND11_MODULE(pyxrt, m) {
m.doc() = "Pybind11 module for XRT";

/*
 *
 * Constants and Enums
 *
 *
 */
m.attr("XCL_BO_FLAGS_NONE") = py::int_(XCL_BO_FLAGS_NONE);

py::enum_<xclBOSyncDirection>(m, "xclBOSyncDirection")
    .value("XCL_BO_SYNC_BO_TO_DEVICE", xclBOSyncDirection::XCL_BO_SYNC_BO_TO_DEVICE)
    .value("XCL_BO_SYNC_BO_FROM_DEVICE", xclBOSyncDirection::XCL_BO_SYNC_BO_FROM_DEVICE);


/*
 *
 * XRT:: Functions
 *
 */
m.def("xclProbe", &xclProbe); 

m.def("xrtDeviceOpen", &xrtDeviceOpen,
    py::arg("index"));

m.def("xrtDeviceClose", &xrtDeviceClose,
    py::arg("dhdl"));

m.def("xrtDeviceLoadXclbin", &xrtDeviceLoadXclbin,
    py::arg("dhdl"),
    py::arg("xclbin"));

m.def("xrtDeviceLoadXclbinFile", &xrtDeviceLoadXclbinFile,
    py::arg("dhdl"),
    py::arg("xclbin_filename"));

m.def("xrtDeviceGetXclbinUUID", &xrtDeviceGetXclbinUUID,
    py::arg("dhld"),
    py::arg("out"));


/*
 *
 * XRT:: UUID (needed since UUID classes passed outside of objects)
 *
 */
py::class_<xrt::uuid>(m, "uuid")
   .def(py::init<char *>())
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
 *
 * XRT::Device
 *
 */
py::class_<xrt::device>(m, "device")
    .def(py::init<>())
    .def(py::init<unsigned int>())
    .def("load_xclbin",
       [](xrt::device & d, const std::string& xclbin){
	     return d.load_xclbin(xclbin);
       }
     )
    .def("get_xclbin_uuid", &xrt::device::get_xclbin_uuid)
    ;


/*
 *
 * XRT::Kernel
 *
 */
m.def("xrtPLKernelOpen", &xrtPLKernelOpen,
    py::arg("deviceHandle"),
    py::arg("xclbinId"),
    py::arg("name"));

m.def("xrtPLKernelOpenExclusive", &xrtPLKernelOpenExclusive,
    py::arg("deviceHandle"),
    py::arg("xclbinId"),
    py::arg("name"));

m.def("xrtKernelClose", &xrtKernelClose,
    py::arg("kernelHandle"));

m.def("xrtKernelArgGroupId", &xrtKernelArgGroupId,
    py::arg("kernelHandle"),
    py::arg("argno"));

m.def("xrtKernelReadRegister", &xrtKernelReadRegister,
    py::arg(""),
    py::arg("offset"),
    py::arg("datap"));

m.def("xrtKernelWriteRegister", &xrtKernelWriteRegister,
    py::arg(""),
    py::arg("offset"),
    py::arg("data"));

m.def("xrtRunOpen", &xrtRunOpen,
    py::arg("kernelHandle"));

m.def("xrtRunStart", &xrtRunStart,
    py::arg("runHandle"));

m.def("xrtRunWait", &xrtRunWait,
    py::arg("runHandle"));

m.def("xrtRunWaitFor", &xrtRunWaitFor,
    py::arg("runHandle"),
    py::arg("timeout_ms"));

m.def("xrtRunState", &xrtRunState,
    py::arg("runHandle"));

m.def("xrtRunSetCallback", &xrtRunSetCallback,
    py::arg("runHandle"),
    py::arg("state"),
    py::arg(""),
    py::arg("data"));

m.def("xrtRunClose", &xrtRunClose,
    py::arg("runHandle"));


py::class_<ert_cmd_state>(m, "ert_cmd_state")
    .def(py::init<>())
    ;

 py::class_<xrt::run>(m, "run")
   .def(py::init<>())
   .def(py::init<const xrt::kernel &>()) 
   .def("start", &xrt::run::start)
   .def("set_arg", [](xrt::run & r, int i, xrt::bo & item){
       r.set_arg(i, item);
     })
   .def("set_arg", [](xrt::run & r, int i, int & item){
       r.set_arg<int>(i, item);
     })  
   .def("wait", [](xrt::run & r) {
       r.wait();
     })
    .def("state", &xrt::run::state)
    .def("add_callback", &xrt::run::add_callback)
    ;

py::class_<xrt::kernel>(m, "kernel")
    .def(py::init([](xrt::device d, const py::array_t<unsigned char> u, const std::string & n, bool e){
        return new xrt::kernel(d, (const unsigned char*) u.request().ptr, n, e);
    }))
  .def("__call__", [](xrt::kernel & k, py::args args) -> xrt::run {
      //  .def("__call__", [](xrt::kernel & k, xrt::bo & bo0, xrt::bo & bo1, int num) -> xrt::run {      
	int i =0;
	xrt::run r(k);
	for (auto item : args) {
	  try 
	    { r.set_arg(i, item.cast<xrt::bo>()); }
	  catch (std::exception e) {  }
	  
	  try 
	    { r.set_arg<int>(i, item.cast<int>()); }
	  catch (std::exception e) {  }
	  
	  i++;
	}	
	r.start();
	return r;


      //return k(bo0, bo1, num);
    })
  .def("setargs_call", [](xrt::kernel & k, xrt::bo & bo0, xrt::bo & bo1, int num) -> xrt::run {
	int i =0;
	xrt::run r(k);
	r.set_arg(0, bo0);
	r.set_arg(1, bo1);
	r.set_arg<int>(2, num);	
	r.start();
	return r;
    })
  .def("setargs_like_operator", [](xrt::kernel & k, py::args args) -> xrt::run { 
	int i =0;
	xrt::run r(k);
	for (auto item : args) {
	  try 
	    { r.set_arg(i, item.cast<xrt::bo>()); }
	  catch (std::exception e) {  }
	  
	  try 
	    { r.set_arg<int>(i, item.cast<int>()); }
	  catch (std::exception e) {  }
	  
	  i++;
	}	
	r.start();
	return r;
    })
    .def("group_id", &xrt::kernel::group_id)
    .def("write_register", &xrt::kernel::write_register)
    .def("read_register", &xrt::kernel::read_register)
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
    py::arg("grp"));

m.def("xrtBOAlloc", &xrtBOAlloc,
    py::arg("dhdl"),
    py::arg("size"),
    py::arg("flags"),
    py::arg("grp"));

m.def("xrtBOSubAlloc", &xrtBOSubAlloc,
    py::arg("parent"),
    py::arg("size"),
    py::arg("offset"));

m.def("xrtBOFree", &xrtBOFree,
    py::arg("handle"));
 
m.def("xrtBOSync", &xrtBOSync,
    py::arg("handle"),
    py::arg("dir"),
    py::arg("size"),
    py::arg("offset"));

m.def("xrtBOMap", &xrtBOMap,
    py::arg("handle"));

m.def("xrtBOWrite", &xrtBOWrite,
    py::arg("handle"),
    py::arg("src"),
    py::arg("size"),
    py::arg("seek"));

m.def("xrtBORead", &xrtBORead,
    py::arg("handle"),
    py::arg("dst"),
    py::arg("size"),
    py::arg("skip"));

py::class_<xrt::bo>(m, "bo")
    .def(py::init<xrt::device,size_t,xrt::buffer_flags,xrt::memory_group>())
    .def("write", ([](xrt::bo &b, py::array_t<int> pyb, size_t seek)  {
	  py::buffer_info info = pyb.request();
	  int* pybptr = (int*) info.ptr;

	  b.write(info.ptr, info.itemsize * info.size , 0);
    }))
    .def("read", ([](xrt::bo &b, size_t size, size_t skip) {
	  int nitems = size/sizeof(int);
	  py::array_t<int> result = py::array_t<int>(nitems);
	  
	  py::buffer_info bufinfo = result.request();
	  int* bufptr = (int*) bufinfo.ptr;
	  b.read(bufptr, size, skip);
	  return result;
     }))
  .def("sync", ([](xrt::bo &b, xclBOSyncDirection dir, size_t size, size_t offset)  {	
	b.sync(dir, size, offset);
      }))
    ;
}
