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


// XRT includes
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"
#include "xrt.h"

// Pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>


/*
class bo_size : public xrt::bo
{
public:
    size_t size;

  XCL_DRIVER_DLLESPEC
  bo_size(xclDeviceHandle dhld, void* userptr, size_t sz, buffer_flags flags, memory_group grp)
    : xrt::bo(dhld, userptr, sz, flags, grp) 
    {
      size = sz;
    }
*/

namespace py = pybind11;


py::array_t<int> add_arrays(py::array_t<int> input1, py::array_t<int> input2) {
    py::buffer_info buf1 = input1.request(), buf2 = input2.request();

    if (buf1.ndim != 1 || buf2.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if (buf1.size != buf2.size)
        throw std::runtime_error("Input shapes must match");

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = py::array_t<int>(buf1.size);

    py::buffer_info buf3 = result.request();

    int *ptr1 = (int *) buf1.ptr,
           *ptr2 = (int *) buf2.ptr,
           *ptr3 = (int *) buf3.ptr;

    for (size_t idx = 0; idx < buf1.shape[0]; idx++)
        ptr3[idx] = ptr1[idx] + ptr2[idx];

    return result;
}

//PYBIND11_MODULE(test, m) {
 
//}


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
    .def(py::init<const xrt::kernel &>()) // init errors
    .def("start", &xrt::run::start)
    .def("wait", &xrt::run::wait,
        py::arg("timeout")=0)
    .def("state", &xrt::run::state)
    .def("add_callback", &xrt::run::add_callback)
    ;

py::class_<xrt::kernel>(m, "kernel")
    .def(py::init([](xrt::device d, const py::array_t<unsigned char> u, const std::string & n, bool e){
        return new xrt::kernel(d, (const unsigned char*) u.request().ptr, n, e);
    }))
    .def("__call__", [](xrt::kernel & k, py::args args){
        return k(args);
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
    .def("sync", &xrt::bo::sync)
    .def("write", ([](xrt::bo &b, py::buffer pyb, size_t seek)  {
        py::buffer_info info = pyb.request();

	int i;
	unsigned char* chptr = (unsigned char*) info.ptr;

	std::cout << "Calling bind::writee: ";
	//	for (int i = 0; i < COUNT; ++i) {
	for (i=0;i<10;++i)
	  std::cout << chptr[i] << "-";
	std::cout << "-- \n";

        b.write(info.ptr, info.itemsize * info.size , 0);
    }))
    .def("write2", ([](xrt::bo &b, py::array_t<int> pyb, size_t seek)  {
        py::buffer_info info = pyb.request();

	int i;
	int* pybptr = (int*) info.ptr;
	pybptr[0] = 8;
	pybptr[9] = 8;
	std::cout << "Calling bind::writee: ";
	//	for (int i = 0; i < COUNT; ++i) {
	for (i=0;i<10;++i)
	  std::cout << pybptr[i] << "-";
	std::cout << "-- \n";
        b.write(info.ptr, info.itemsize * info.size , 0);
    }))
    .def("read", ([](xrt::bo &b, size_t size, size_t skip) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) bufinfo.ptr;
        b.read(bufptr, size, skip);
        return result;
     }))
    .def("read2", ([](xrt::bo &b, size_t size, size_t skip) {

	  int nitems = size/sizeof(int);
	  py::array_t<int> result = py::array_t<int>(nitems);
	  py::buffer_info bufinfo = result.request();
	  int* bufptr = (int*) bufinfo.ptr;
	  b.read(bufptr, size, skip);
	  return result;
     }))
    .def("map", ([](xrt::bo &b, size_t size) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) b.map();
        return result;
     }))
    ;
   m.def("add_arrays", &add_arrays, "Add two NumPy arrays");
}

/*
py::class_<xrt::bo>(m, "bo")
    .def(py::init<xrt::device,size_t,xrt::buffer_flags,xrt::memory_group>())
    .def("sync", &xrt::bo::sync)
    .def("write", ([](xrt::bo &b, py::buffer pyb, size_t seek)  {
        py::buffer_info info = pyb.request();
        b.write(info.ptr, info.itemsize * info.size , 0);
    }))
    .def("read", ([](xrt::bo &b, size_t size, size_t skip) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) bufinfo.ptr;
        b.read(bufptr, size, skip);
        return result;
     }))
    .def("map", ([](xrt::bo &b, size_t size) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) b.map();
        return result;
     }))
    ;
}
*/



/*
py::class_<bo_size>(m, "bo_size")
    .def(py::init<xrt::device,size_t,xrt::buffer_flags,xrt::memory_group>())
    .def("sync", &xrt::bo::sync)
    .def("write", ([](xrt::bo &b, py::buffer pyb, size_t seek)  {
        py::buffer_info info = pyb.request();
        b.write(info.ptr, info.itemsize * info.size , 0);
    }))
    .def("read", ([](xrt::bo &b, size_t size, size_t skip) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) bufinfo.ptr;
        b.read(bufptr, size, skip);
        return result;
     }))
    .def("map", ([](xrt::bo &b, size_t size) {
        py::array_t<unsigned char> result = py::array_t<unsigned char>(size);
        py::buffer_info bufinfo = result.request();
        unsigned char* bufptr = (unsigned char*) b.map();
        return result;
     }))
    ;
}
*/
