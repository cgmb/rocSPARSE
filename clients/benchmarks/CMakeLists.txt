# ########################################################################
# Copyright (c) 2018-2020 Advanced Micro Devices, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# ########################################################################

set(Boost_USE_MULTITHREADED ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_ADDITIONAL_VERSIONS 1.65.1 1.65)
set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost COMPONENTS program_options)

if(NOT Boost_FOUND)
  message(STATUS "Dynamic boost libraries not found. Attempting to find static libraries...")
  set(Boost_USE_STATIC_LIBS ON)
  find_package(Boost REQUIRED COMPONENTS program_options)
endif()

set(ROCSPARSE_BENCHMARK_SOURCES
  client.cpp
)

set(ROCSPARSE_CLIENTS_COMMON
  ../common/utility.cpp
  ../common/rocsparse_template_specialization.cpp
  ../common/rocsparse_init.cpp
  ../common/rocsparse_host.cpp
)


set(ROCSPARSE_CLIENTS_TESTINGS
../testings/testing_axpyi.cpp
../testings/testing_doti.cpp
../testings/testing_dotci.cpp
../testings/testing_gthr.cpp
../testings/testing_gthrz.cpp
../testings/testing_roti.cpp
../testings/testing_sctr.cpp
../testings/testing_bsrmv.cpp
../testings/testing_bsrsv.cpp
../testings/testing_coomv.cpp
../testings/testing_csrmv.cpp
../testings/testing_csrsv.cpp
../testings/testing_ellmv.cpp
../testings/testing_hybmv.cpp
../testings/testing_bsrmm.cpp
../testings/testing_csrmm.cpp
../testings/testing_csrsm.cpp
../testings/testing_gemmi.cpp
../testings/testing_csrgeam.cpp
../testings/testing_csrgemm.cpp
../testings/testing_bsric0.cpp
../testings/testing_bsrilu0.cpp
../testings/testing_csric0.cpp
../testings/testing_csrilu0.cpp
../testings/testing_csr2coo.cpp
../testings/testing_csr2csc.cpp
../testings/testing_gebsr2gebsc.cpp
../testings/testing_gebsr2gebsr.cpp
../testings/testing_csr2ell.cpp
../testings/testing_csr2hyb.cpp
../testings/testing_csr2bsr.cpp
../testings/testing_csr2gebsr.cpp
../testings/testing_coo2csr.cpp
../testings/testing_ell2csr.cpp
../testings/testing_hyb2csr.cpp
../testings/testing_bsr2csr.cpp
../testings/testing_gebsr2csr.cpp
../testings/testing_csr2csr_compress.cpp
../testings/testing_prune_csr2csr.cpp
../testings/testing_prune_csr2csr_by_percentage.cpp
../testings/testing_identity.cpp
../testings/testing_csrsort.cpp
../testings/testing_cscsort.cpp
../testings/testing_coosort.cpp
../testings/testing_csricsv.cpp
../testings/testing_csrilusv.cpp
../testings/testing_nnz.cpp
../testings/testing_dense2csr.cpp
../testings/testing_prune_dense2csr.cpp
../testings/testing_prune_dense2csr_by_percentage.cpp
../testings/testing_dense2csc.cpp
../testings/testing_csr2dense.cpp
../testings/testing_csc2dense.cpp
)

add_executable(rocsparse-bench ${ROCSPARSE_BENCHMARK_SOURCES} ${ROCSPARSE_CLIENTS_COMMON} ${ROCSPARSE_CLIENTS_TESTINGS})

# Target compile options
target_compile_options(rocsparse-bench PRIVATE -Wno-unused-command-line-argument -Wall)

# Internal common header
target_include_directories(rocsparse-bench PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)

# Target link libraries
target_link_libraries(rocsparse-bench PRIVATE Boost::program_options roc::rocsparse hip::host)

# Add OpenMP if available
if(OPENMP_FOUND)
  target_link_libraries(rocsparse-bench PRIVATE OpenMP::OpenMP_CXX -Wl,-rpath=${HIP_CLANG_ROOT}/lib)
endif()

# Set benchmark output directory
set_target_properties(rocsparse-bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/staging")
