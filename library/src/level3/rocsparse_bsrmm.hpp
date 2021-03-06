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

#pragma once
#ifndef ROCSPARSE_BSRMM_HPP
#define ROCSPARSE_BSRMM_HPP

#include "templates.h"
#include "utility.h"
#include <hip/hip_runtime.h>

template <typename T, typename U>
rocsparse_status rocsparse_bsrmm_template_small(rocsparse_handle          handle,
                                                rocsparse_direction       dir,
                                                rocsparse_operation       trans_A,
                                                rocsparse_operation       trans_B,
                                                rocsparse_int             mb,
                                                rocsparse_int             n,
                                                rocsparse_int             kb,
                                                rocsparse_int             nnzb,
                                                U                         alpha,
                                                const rocsparse_mat_descr descr,
                                                const T*                  bsr_val,
                                                const rocsparse_int*      bsr_row_ptr,
                                                const rocsparse_int*      bsr_col_ind,
                                                rocsparse_int             block_dim,
                                                const T*                  B,
                                                rocsparse_int             ldb,
                                                U                         beta,
                                                T*                        C,
                                                rocsparse_int             ldc);

template <typename T, typename U>
rocsparse_status rocsparse_bsrmm_template_large(rocsparse_handle          handle,
                                                rocsparse_direction       dir,
                                                rocsparse_operation       trans_A,
                                                rocsparse_operation       trans_B,
                                                rocsparse_int             mb,
                                                rocsparse_int             n,
                                                rocsparse_int             kb,
                                                rocsparse_int             nnzb,
                                                U                         alpha,
                                                const rocsparse_mat_descr descr,
                                                const T*                  bsr_val,
                                                const rocsparse_int*      bsr_row_ptr,
                                                const rocsparse_int*      bsr_col_ind,
                                                rocsparse_int             block_dim,
                                                const T*                  B,
                                                rocsparse_int             ldb,
                                                U                         beta,
                                                T*                        C,
                                                rocsparse_int             ldc);

template <typename T, typename U>
rocsparse_status rocsparse_bsrmm_template_large_ext(rocsparse_handle          handle,
                                                    rocsparse_direction       dir,
                                                    rocsparse_operation       trans_A,
                                                    rocsparse_operation       trans_B,
                                                    rocsparse_int             mb,
                                                    rocsparse_int             n,
                                                    rocsparse_int             kb,
                                                    rocsparse_int             nnzb,
                                                    U                         alpha,
                                                    const rocsparse_mat_descr descr,
                                                    const T*                  bsr_val,
                                                    const rocsparse_int*      bsr_row_ptr,
                                                    const rocsparse_int*      bsr_col_ind,
                                                    rocsparse_int             block_dim,
                                                    const T*                  B,
                                                    rocsparse_int             ldb,
                                                    U                         beta,
                                                    T*                        C,
                                                    rocsparse_int             ldc);

template <typename T, typename U>
rocsparse_status rocsparse_bsrmm_template_general(rocsparse_handle          handle,
                                                  rocsparse_direction       dir,
                                                  rocsparse_operation       trans_A,
                                                  rocsparse_operation       trans_B,
                                                  rocsparse_int             mb,
                                                  rocsparse_int             n,
                                                  rocsparse_int             kb,
                                                  rocsparse_int             nnzb,
                                                  U                         alpha,
                                                  const rocsparse_mat_descr descr,
                                                  const T*                  bsr_val,
                                                  const rocsparse_int*      bsr_row_ptr,
                                                  const rocsparse_int*      bsr_col_ind,
                                                  rocsparse_int             block_dim,
                                                  const T*                  B,
                                                  rocsparse_int             ldb,
                                                  U                         beta,
                                                  T*                        C,
                                                  rocsparse_int             ldc);

template <typename T, typename U>
rocsparse_status rocsparse_bsrmm_template_dispatch(rocsparse_handle          handle,
                                                   rocsparse_direction       dir,
                                                   rocsparse_operation       trans_A,
                                                   rocsparse_operation       trans_B,
                                                   rocsparse_int             mb,
                                                   rocsparse_int             n,
                                                   rocsparse_int             kb,
                                                   rocsparse_int             nnzb,
                                                   U                         alpha,
                                                   const rocsparse_mat_descr descr,
                                                   const T*                  bsr_val,
                                                   const rocsparse_int*      bsr_row_ptr,
                                                   const rocsparse_int*      bsr_col_ind,
                                                   rocsparse_int             block_dim,
                                                   const T*                  B,
                                                   rocsparse_int             ldb,
                                                   U                         beta,
                                                   T*                        C,
                                                   rocsparse_int             ldc)
{
    if(block_dim == 2)
    {
        return rocsparse_bsrmm_template_small(handle,
                                              dir,
                                              trans_A,
                                              trans_B,
                                              mb,
                                              n,
                                              kb,
                                              nnzb,
                                              alpha,
                                              descr,
                                              bsr_val,
                                              bsr_row_ptr,
                                              bsr_col_ind,
                                              block_dim,
                                              B,
                                              ldb,
                                              beta,
                                              C,
                                              ldc);
    }
    else if(block_dim <= 32)
    {
        return rocsparse_bsrmm_template_large_ext(handle,
                                                  dir,
                                                  trans_A,
                                                  trans_B,
                                                  mb,
                                                  n,
                                                  kb,
                                                  nnzb,
                                                  alpha,
                                                  descr,
                                                  bsr_val,
                                                  bsr_row_ptr,
                                                  bsr_col_ind,
                                                  block_dim,
                                                  B,
                                                  ldb,
                                                  beta,
                                                  C,
                                                  ldc);
    }
    else
    {
        return rocsparse_bsrmm_template_general(handle,
                                                dir,
                                                trans_A,
                                                trans_B,
                                                mb,
                                                n,
                                                kb,
                                                nnzb,
                                                alpha,
                                                descr,
                                                bsr_val,
                                                bsr_row_ptr,
                                                bsr_col_ind,
                                                block_dim,
                                                B,
                                                ldb,
                                                beta,
                                                C,
                                                ldc);
    }
}

template <typename T>
rocsparse_status rocsparse_bsrmm_template(rocsparse_handle          handle,
                                          rocsparse_direction       dir,
                                          rocsparse_operation       trans_A,
                                          rocsparse_operation       trans_B,
                                          rocsparse_int             mb,
                                          rocsparse_int             n,
                                          rocsparse_int             kb,
                                          rocsparse_int             nnzb,
                                          const T*                  alpha,
                                          const rocsparse_mat_descr descr,
                                          const T*                  bsr_val,
                                          const rocsparse_int*      bsr_row_ptr,
                                          const rocsparse_int*      bsr_col_ind,
                                          rocsparse_int             block_dim,
                                          const T*                  B,
                                          rocsparse_int             ldb,
                                          const T*                  beta,
                                          T*                        C,
                                          rocsparse_int             ldc)
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(descr == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging TODO bench logging
    if(handle->pointer_mode == rocsparse_pointer_mode_host)
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xbsrmm"),
                  dir,
                  trans_A,
                  trans_B,
                  mb,
                  n,
                  kb,
                  nnzb,
                  *alpha,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  block_dim,
                  (const void*&)B,
                  ldb,
                  *beta,
                  (const void*&)C,
                  ldc);
    }
    else
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xbsrmm"),
                  dir,
                  trans_A,
                  trans_B,
                  mb,
                  n,
                  kb,
                  nnzb,
                  (const void*&)alpha,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  block_dim,
                  (const void*&)B,
                  ldb,
                  (const void*&)beta,
                  (const void*&)C,
                  ldc);
    }

    // Check index base
    if(descr->base != rocsparse_index_base_zero && descr->base != rocsparse_index_base_one)
    {
        return rocsparse_status_invalid_value;
    }

    // Check matrix type
    if(descr->type != rocsparse_matrix_type_general)
    {
        // TODO
        return rocsparse_status_not_implemented;
    }

    // Check operation
    if(trans_A != rocsparse_operation_none)
    {
        return rocsparse_status_not_implemented;
    }
    else if(trans_B != rocsparse_operation_none && trans_B != rocsparse_operation_transpose)
    {
        return rocsparse_status_not_implemented;
    }

    // Check sizes
    if(mb < 0 || n < 0 || kb < 0 || nnzb < 0 || block_dim <= 0)
    {
        return rocsparse_status_invalid_size;
    }

    // Quick return if possible
    if(mb == 0 || n == 0 || kb == 0)
    {
        return rocsparse_status_success;
    }

    // Check pointer arguments
    if(bsr_val == nullptr || bsr_row_ptr == nullptr || bsr_col_ind == nullptr || B == nullptr
       || C == nullptr || alpha == nullptr || beta == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Check leading dimension of B
    if(trans_B == rocsparse_operation_none)
    {
        if(ldb < kb)
        {
            return rocsparse_status_invalid_size;
        }
    }
    else
    {
        if(ldb < n)
        {
            return rocsparse_status_invalid_size;
        }
    }

    // Check leading dimension of C
    if(ldc < mb)
    {
        return rocsparse_status_invalid_size;
    }

    // Stream

    // If n is only 1 and B are non-transposed, then call bsrmv
    if(n == 1)
    {
        if(trans_B == rocsparse_operation_none)
        {

            return rocsparse_bsrmv(handle,
                                   dir,
                                   trans_A,
                                   mb,
                                   kb,
                                   nnzb,
                                   alpha,
                                   descr,
                                   bsr_val,
                                   bsr_row_ptr,
                                   bsr_col_ind,
                                   block_dim,
                                   B,
                                   beta,
                                   C);
        }
    }

    // If block dimension is one we can simply call csrmm
    if(block_dim == 1)
    {
        rocsparse_int nnz = nnzb * block_dim;
        rocsparse_int m   = mb * block_dim;
        rocsparse_int k   = kb * block_dim;
        return rocsparse_csrmm(handle,
                               trans_A,
                               trans_B,
                               m,
                               n,
                               k,
                               nnz,
                               alpha,
                               descr,
                               bsr_val,
                               bsr_row_ptr,
                               bsr_col_ind,
                               B,
                               ldb,
                               beta,
                               C,
                               ldc);
    }

    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {
        return rocsparse_bsrmm_template_dispatch(handle,
                                                 dir,
                                                 trans_A,
                                                 trans_B,
                                                 mb,
                                                 n,
                                                 kb,
                                                 nnzb,
                                                 alpha,
                                                 descr,
                                                 bsr_val,
                                                 bsr_row_ptr,
                                                 bsr_col_ind,
                                                 block_dim,
                                                 B,
                                                 ldb,
                                                 beta,
                                                 C,
                                                 ldc);
    }
    else
    {
        return rocsparse_bsrmm_template_dispatch(handle,
                                                 dir,
                                                 trans_A,
                                                 trans_B,
                                                 mb,
                                                 n,
                                                 kb,
                                                 nnzb,
                                                 *alpha,
                                                 descr,
                                                 bsr_val,
                                                 bsr_row_ptr,
                                                 bsr_col_ind,
                                                 block_dim,
                                                 B,
                                                 ldb,
                                                 *beta,
                                                 C,
                                                 ldc);
    }
}

#endif // ROCSPARSE_BSRMM_HPP
