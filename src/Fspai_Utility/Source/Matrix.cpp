/*
    =======================================================================
    =======================================================================
    ==                                                                   ==
    ==  FSPAI:  Factorized SPAI algorithm to compute a Factorized SParse ==
    ==          Approximate Inverse matrix for symmetric positive        ==
    ==          definite systems.                                        ==
    ==                                                                   ==
    ==  Copyright (C)  2011 by                                           ==
    ==                 Matous Sedlacek <sedlacek@in.tum.de>              ==
    ==                 Chair of Scientific Computing -- Informatics V    ==
    ==                 Technische Universität München                    ==
    ==                                                                   ==
    ==  This file is part of FSPAI.                                      ==
    ==                                                                   ==
    ==  FSPAI is free software: you can redistribute it and/or           ==
    ==  modify it under the terms of the GNU Lesser General Public       ==
    ==  License as published by the Free Software Foundation, either     ==
    ==  version 3 of the License, or (at your option) any later version. ==
    ==                                                                   ==
    ==  FSPAI is distributed in the hope that it will be useful,         ==
    ==  but WITHOUT ANY WARRANTY; without even the implied warranty of   ==
    ==  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    ==
    ==  GNU Lesser General Public License for more details.              ==
    ==                                                                   ==
    ==  You should have received a copy of the GNU Lesser General        ==
    ==  Public License along with FSPAI.                                 ==
    ==  If not, see <http://www.gnu.org/licenses/>.                      ==
    ==                                                                   ==
    =======================================================================
    =======================================================================
*/

//file includings
#include "../Include/Matrix.h"

//C++ includings
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>


//============================================================================
//============================================================================
//================ Template specifications for double matrices ===============
//============================================================================
//============================================================================

template<> void
Matrix<double>::Print_Matrix_Data
( ) const
{
    /*
    std::cout << "\n\tMatrix Data:\t\n"
            << "\n\tmy_id:\t\t" << my_id
            << "\n\tnum_procs:\t" << num_procs
            << "\n\tmy_nbr_cols:\t" << my_nbr_cols
            << "\n\tmy_start_idx:\t" << my_start_idx
            << "\n\tn:\t\t" << n
            << "\n\tm:\t\t" << m
            << "\n\tmy_nnz:\t\t" << my_nnz
            << "\n\tmax_nnz:\t" << max_nnz
            << std::endl;

    std::cout << "\tlen_all_cols:\t";
    for (int i = 0; i < n; i++)
        std::cout << len_all_cols[i] << " ";
    std::cout << std::endl;

    std::cout << "\tall_nbr_cols:\t";
    for (int i = 0; i < num_procs; i++)
        std::cout << all_nbr_cols[i] << " ";
    std::cout << std::endl;

    std::cout << "\tstart_indices:\t";
    for (int i = 0; i < num_procs; i++)
        std::cout << start_indices[i] << " ";
    std::cout << std::endl;

    std::cout << "\tpe:\t\t";
    for (int i = 0; i < n; i++)
        std::cout << pe[i] << " ";
    std::cout << std::endl;

    for (int i = 0; i < my_nbr_cols; i++)
    {
        int l = c_lines->len_cols[i];
        std::cout << "\n\tlen_cols[" << i << "]:\t" << l << std::endl;
        std::cout << "\t";
        std::cout << "A_buf:\t\t";
        for (int j = 0; j < l; j++)
            std::cout << c_lines->mtx_vals[i][j] << " ";
        std::cout << std::endl;
        std::cout << "\t";
        std::cout << "col_idcs:\t";
        for (int j = 0; j < l; j++)
            std::cout << c_lines->col_idcs[i][j] << " ";
        std::cout << std::endl;
    }
   */
    return;
}



template<>  void
Matrix<double>::Print_Matrix_Human_Readable
( ) const
{
    /*
    double* print_matrix = new double [n * m],
            val;

    memset(print_matrix, 0, n * m *sizeof(double));

    for (int j = 0; j < n; j++)
        for (int i = 0; i < c_lines->len_cols[j]; i++)
            print_matrix[j * m + c_lines->col_idcs[j][i]]
                    = c_lines->mtx_vals[j][i];

    std::cout << "\n\t\tMatrix human readable:\t\n"<< std::endl;
    std::cout << "\t\t       ";
    for (int j = 0; j < n; j++)
        std:: cout << "     " << j << "     ";
    std::cout << "\n" << std::endl;

    for (int i = 0; i < m; i++)
    {
        std::cout << "\t\t" <<  i << "     ";
        for (int j = 0; j < n; j++)
        {
            val = print_matrix[j * m + i];
            if (fabs(val) < 1.0e-10)
            {
                if (val < 0)
                    printf("|        %d ", 0);
                else
                    printf("|        %d ", 0);
            }
            else
            {
                if (val < 0)
                    printf("| %2.5f ", val);
                else
                    printf("|  %2.5f ", val);
            }
        }
        std::cout << "|\n" << std::endl;
    }
    std::cout << "\n" << std::endl;

    delete [] print_matrix;
    */
    return;
}


template<> void
Matrix<double>::Write_Line
(   int c,
    int r,
    FILE* f ) const
{
    int     row = c_lines->col_idcs[c][r] + 1,
            col = c + my_start_idx + 1;
    double  val = c_lines->mtx_vals[c][r];

    // Write data
    fprintf(f, "%d %d %.13e\n", row, col, val);
}



template<> void
Matrix<double>::Write_Header
(   FILE* f ) const
{
    fprintf(f, "matrix coordinate real symmetric\n");
}

//============================================================================
//============================================================================
//=============== Template specifications for COMPLEX matrices ===============
//============================================================================
//============================================================================


template<> void
Matrix<COMPLEX>::Print_Matrix_Data
( ) const
{   /*
    std::cout << "\n\tMatrix Data:\t\n"
            << "\n\tmy_id:\t\t" << my_id
            << "\n\tnum_procs:\t" << num_procs
            << "\n\tmy_nbr_cols:\t" << my_nbr_cols
            << "\n\tmy_start_idx:\t" << my_start_idx
            << "\n\tn:\t\t" << n
            << "\n\tm:\t\t" << m
            << "\n\tmy_nnz:\t\t" << my_nnz
            << std::endl;

    std::cout << "\tlen_all_cols:\t";
    for (int i = 0; i < n; i++)
        std::cout << len_all_cols[i] << " ";
    std::cout << std::endl;

    std::cout << "\tall_nbr_cols:\t";
    for (int i = 0; i < num_procs; i++)
        std::cout << all_nbr_cols[i] << " ";
    std::cout << std::endl;

    std::cout << "\tstart_indices:\t";
    for (int i = 0; i < num_procs; i++)
        std::cout << start_indices[i] << " ";
    std::cout << std::endl;

    std::cout << "\tpe:\t\t";
    for (int i = 0; i < n; i++)
        std::cout << pe[i] << " ";
    std::cout << std::endl;

    for (int i = 0; i < my_nbr_cols; i++)
    {
        int l = c_lines->len_cols[i];
        std::cout << "\n\tlen_cols[" << i << "]:\t" << l << std::endl;
        std::cout << "\t";
        std::cout << "mtx_buf:\t\t";
        for (int j = 0; j < l; j++)
        {
            if (c_lines->mtx_vals[i][j].imag < 0.0)
                std::cout << c_lines->mtx_vals[i][j].real
                          << c_lines->mtx_vals[i][j].imag << "i | ";
            else
                std::cout << c_lines->mtx_vals[i][j].real << "+"
                          << c_lines->mtx_vals[i][j].imag << "i | ";
        }
        std::cout << std::endl;
        std::cout << "\t";
        std::cout << "col_idcs:\t";
        for (int j = 0; j < l; j++)
            std::cout << c_lines->col_idcs[i][j] << " ";
        std::cout << std::endl;
    }

  */
  return;
}



template<>  void
Matrix<COMPLEX>::Print_Matrix_Human_Readable
( ) const
{   /*
    COMPLEX*    print_matrix = new COMPLEX [n * m];
    double      real, imag;

    memset(print_matrix, 0, n * m *sizeof(COMPLEX));

    for (int j = 0; j < n; j++)
        for (int i = 0; i < c_lines->len_cols[j]; i++)
            print_matrix[j * m + c_lines->col_idcs[j][i]]
                = c_lines->mtx_vals[j][i];

    std::cout << "\n\t\tMatrix human readable:\t\n"<< std::endl;
    std::cout << "\t\t       ";
    for (int j = 0; j < n; j++)
        std:: cout << "          " << j << "          ";
    std::cout << "\n" << std::endl;

    for (int i = 0; i < m; i++)
    {
        std::cout << "\t\t" <<  i << "     ";
        for (int j = 0; j < n; j++)
        {
            real = print_matrix[j * m + i].real;
            imag = print_matrix[j * m + i].imag;
            if (real < 0)
            {
                if (imag < 0.0)
                    printf("| %2.5f %2.5fi ", real, imag);
                else
                    printf("| %2.5f +%2.5fi ", real, imag);
            }
            else
            {
                if (imag < 0.0)
                    printf("|  %2.5f %2.5fi ", real, imag);
                else
                    printf("|  %2.5f +%2.5fi ", real, imag);
            }
        }
        std::cout << "|\n" << std::endl;
    }
    std::cout << "\n" << std::endl;

    delete [] print_matrix;
    */
    return;
}



template<> void
Matrix<COMPLEX>::Write_Line
(   int     c,
    int     r,
    FILE*   f ) const
{
    int     row   = c_lines->col_idcs[c][r] + 1,
            col   = c + my_start_idx + 1;
    double  real  = c_lines->mtx_vals[c][r].real,
            imag  = c_lines->mtx_vals[c][r].imag;

    // Write data
    //fprintf(f, "%d %d %.13e %.13e\n", row, col, real, imag);

    return;
}



template<> void
Matrix<COMPLEX>::Write_Header
(   FILE* f ) const
{
    //fprintf(f, "matrix coordinate complex symmetric\n");
    return;
}
