/*! \file */
/* ************************************************************************
 * Copyright (c) 2020 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

#include "rocsparse_bsrmv.hpp"
#include "rocsparse_bsrmv_spzl.hpp"
#include "rocsparse_csrmv.hpp"

template <typename T, typename U>
rocsparse_status rocsparse_bsrmv_template_dispatch(rocsparse_handle          handle,
                                                   rocsparse_direction       dir,
                                                   rocsparse_operation       trans,
                                                   rocsparse_int             mb,
                                                   rocsparse_int             nb,
                                                   rocsparse_int             nnzb,
                                                   U                         alpha_device_host,
                                                   const rocsparse_mat_descr descr,
                                                   const T*                  bsr_val,
                                                   const rocsparse_int*      bsr_row_ptr,
                                                   const rocsparse_int*      bsr_col_ind,
                                                   rocsparse_int             bsr_dim,
                                                   const T*                  x,
                                                   U                         beta_device_host,
                                                   T*                        y)
{

    // Run different bsrmv kernels
    if(trans == rocsparse_operation_none)
    {
        if(handle->wavefront_size == 32)
        {
            bsrmvn_general(handle,
                           dir,
                           mb,
                           alpha_device_host,
                           bsr_row_ptr,
                           bsr_col_ind,
                           bsr_val,
                           bsr_dim,
                           x,
                           beta_device_host,
                           y,
                           descr->base);

            return rocsparse_status_success;
        }

        if(bsr_dim == 2)
        {
            bsrmvn_2x2(handle,
                       dir,
                       mb,
                       nnzb,
                       alpha_device_host,
                       bsr_row_ptr,
                       bsr_col_ind,
                       bsr_val,
                       x,
                       beta_device_host,
                       y,
                       descr->base);
        }
        else if(bsr_dim == 3)
        {
            bsrmvn_3x3(handle,
                       dir,
                       mb,
                       nnzb,
                       alpha_device_host,
                       bsr_row_ptr,
                       bsr_col_ind,
                       bsr_val,
                       x,
                       beta_device_host,
                       y,
                       descr->base);
        }
        else if(bsr_dim == 4)
        {
            bsrmvn_4x4(handle,
                       dir,
                       mb,
                       nnzb,
                       alpha_device_host,
                       bsr_row_ptr,
                       bsr_col_ind,
                       bsr_val,
                       x,
                       beta_device_host,
                       y,
                       descr->base);
        }
        else if(bsr_dim == 5)
        {
            bsrmvn_5x5(handle,
                       dir,
                       mb,
                       nnzb,
                       alpha_device_host,
                       bsr_row_ptr,
                       bsr_col_ind,
                       bsr_val,
                       x,
                       beta_device_host,
                       y,
                       descr->base);
        }
        else if(bsr_dim == 8)
        {
            bsrmvn_8x8(handle,
                       dir,
                       mb,
                       nnzb,
                       alpha_device_host,
                       bsr_row_ptr,
                       bsr_col_ind,
                       bsr_val,
                       x,
                       beta_device_host,
                       y,
                       descr->base);
        }
        else if(bsr_dim == 16)
        {
            bsrmvn_16x16(handle,
                         dir,
                         mb,
                         nnzb,
                         alpha_device_host,
                         bsr_row_ptr,
                         bsr_col_ind,
                         bsr_val,
                         x,
                         beta_device_host,
                         y,
                         descr->base);
        }
        else if(bsr_dim > 16 && bsr_dim <= 32)
        {

            bsrmvn_17_32(handle,
                         dir,
                         mb,
                         nnzb,
                         alpha_device_host,
                         bsr_row_ptr,
                         bsr_col_ind,
                         bsr_val,
                         bsr_dim,
                         x,
                         beta_device_host,
                         y,
                         descr->base);
        }
        else
        {
            bsrmvn_general(handle,
                           dir,
                           mb,
                           alpha_device_host,
                           bsr_row_ptr,
                           bsr_col_ind,
                           bsr_val,
                           bsr_dim,
                           x,
                           beta_device_host,
                           y,
                           descr->base);
        }
    }
    else
    {
        // TODO
        return rocsparse_status_not_implemented;
    }

    return rocsparse_status_success;
}

template <typename T>
rocsparse_status rocsparse_bsrmv_template(rocsparse_handle          handle,
                                          rocsparse_direction       dir,
                                          rocsparse_operation       trans,
                                          rocsparse_int             mb,
                                          rocsparse_int             nb,
                                          rocsparse_int             nnzb,
                                          const T*                  alpha_device_host,
                                          const rocsparse_mat_descr descr,
                                          const T*                  bsr_val,
                                          const rocsparse_int*      bsr_row_ptr,
                                          const rocsparse_int*      bsr_col_ind,
                                          rocsparse_int             bsr_dim,
                                          const T*                  x,
                                          const T*                  beta_device_host,
                                          T*                        y)
{

    //
    // Check for valid handle and matrix descriptor
    //
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }

    if(descr == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    //
    // Logging
    //
    if(handle->pointer_mode == rocsparse_pointer_mode_host)
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xbsrmv"),
                  dir,
                  trans,
                  mb,
                  nb,
                  nnzb,
                  *alpha_device_host,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  bsr_dim,
                  (const void*&)x,
                  *beta_device_host,
                  (const void*&)y);

        log_bench(handle,
                  "./rocsparse-bench -f bsrmv -r",
                  replaceX<T>("X"),
                  "--mtx <matrix.mtx> "
                  "--bsrdim",
                  bsr_dim,
                  "--alpha",
                  *alpha_device_host,
                  "--beta",
                  *beta_device_host);
    }
    else
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xbsrmv"),
                  dir,
                  trans,
                  mb,
                  nb,
                  nnzb,
                  (const void*&)alpha_device_host,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  bsr_dim,
                  (const void*&)x,
                  (const void*&)beta_device_host,
                  (const void*&)y);
    }

    //
    // Check index base
    //
    if(descr->base != rocsparse_index_base_zero && descr->base != rocsparse_index_base_one)
    {
        return rocsparse_status_invalid_value;
    }

    if(descr->type != rocsparse_matrix_type_general)
    {
        //
        // TODO ?
        //
        return rocsparse_status_not_implemented;
    }

    //
    // Check sizes
    //
    if(mb < 0 || nb < 0 || nnzb < 0 || bsr_dim < 0)
    {
        return rocsparse_status_invalid_size;
    }

    //
    // Quick return if possible
    //
    if(mb == 0 || nb == 0 || nnzb == 0 || bsr_dim == 0)
    {
        return rocsparse_status_success;
    }

    //
    // Check pointer arguments
    //
    if(alpha_device_host == nullptr || beta_device_host == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    //
    // Another quick return.
    //
    if(handle->pointer_mode == rocsparse_pointer_mode_host
       && *alpha_device_host == static_cast<T>(0) && *beta_device_host == static_cast<T>(1))
    {
        return rocsparse_status_success;
    }

    //
    // Check the rest of pointer arguments
    //
    if(bsr_val == nullptr || bsr_row_ptr == nullptr || bsr_col_ind == nullptr || x == nullptr
       || y == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    //
    // bsr_dim == 1 is the CSR case
    //
    if(bsr_dim == 1)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_csrmv_template(handle,
                                                           trans,
                                                           mb,
                                                           nb,
                                                           nnzb,
                                                           alpha_device_host,
                                                           descr,
                                                           bsr_val,
                                                           bsr_row_ptr,
                                                           bsr_col_ind,
                                                           nullptr,
                                                           x,
                                                           beta_device_host,
                                                           y));

        return rocsparse_status_success;
    }

    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {
        return rocsparse_bsrmv_template_dispatch(handle,
                                                 dir,
                                                 trans,
                                                 mb,
                                                 nb,
                                                 nnzb,
                                                 alpha_device_host,
                                                 descr,
                                                 bsr_val,
                                                 bsr_row_ptr,
                                                 bsr_col_ind,
                                                 bsr_dim,
                                                 x,
                                                 beta_device_host,
                                                 y);
    }
    else
    {
        return rocsparse_bsrmv_template_dispatch(handle,
                                                 dir,
                                                 trans,
                                                 mb,
                                                 nb,
                                                 nnzb,
                                                 *alpha_device_host,
                                                 descr,
                                                 bsr_val,
                                                 bsr_row_ptr,
                                                 bsr_col_ind,
                                                 bsr_dim,
                                                 x,
                                                 *beta_device_host,
                                                 y);
    }
}

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */

#define C_IMPL(NAME, TYPE)                                                  \
    extern "C" rocsparse_status NAME(rocsparse_handle          handle,      \
                                     rocsparse_direction       dir,         \
                                     rocsparse_operation       trans,       \
                                     rocsparse_int             mb,          \
                                     rocsparse_int             nb,          \
                                     rocsparse_int             nnzb,        \
                                     const TYPE*               alpha,       \
                                     const rocsparse_mat_descr descr,       \
                                     const TYPE*               bsr_val,     \
                                     const rocsparse_int*      bsr_row_ptr, \
                                     const rocsparse_int*      bsr_col_ind, \
                                     rocsparse_int             bsr_dim,     \
                                     const TYPE*               x,           \
                                     const TYPE*               beta,        \
                                     TYPE*                     y)           \
    {                                                                       \
        return rocsparse_bsrmv_template(handle,                             \
                                        dir,                                \
                                        trans,                              \
                                        mb,                                 \
                                        nb,                                 \
                                        nnzb,                               \
                                        alpha,                              \
                                        descr,                              \
                                        bsr_val,                            \
                                        bsr_row_ptr,                        \
                                        bsr_col_ind,                        \
                                        bsr_dim,                            \
                                        x,                                  \
                                        beta,                               \
                                        y);                                 \
    }

C_IMPL(rocsparse_sbsrmv, float);
C_IMPL(rocsparse_dbsrmv, double);
C_IMPL(rocsparse_cbsrmv, rocsparse_float_complex);
C_IMPL(rocsparse_zbsrmv, rocsparse_double_complex);

#undef C_IMPL
