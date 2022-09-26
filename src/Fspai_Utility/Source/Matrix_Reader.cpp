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
#include "../Include/Matrix_Reader.h"


//============================================================================
//============================================================================
//================ Template specifications for double matrices ===============
//============================================================================
//============================================================================


template<> void
Matrix_Reader<double, RCV>::Fill_Field_Date
(   const int       row,
    const int       col,
    const double    val,
    int&            inc)
{
    cols[inc].i = row;
    cols[inc].j = col;
    cols[inc].val = val;
    inc++;
}



template<> void
Matrix_Reader<double, RCV>::Data_To_Memory
(   FILE    *f )
{
    int     row,
            col,
            lencol= 0,
            pos = 0;
    double  val;
    char    line[128];

    diagonal_elements = new bool[my_nbr_cols];
    memset(diagonal_elements, false, my_nbr_cols*sizeof(bool));

    for ( int i = 0; i < file_nnz; i++ )
    {
        if ( !fgets(line,128,f) ) break;
        // Change ',' to ' '
        for ( int c = 0; line[c]; c++ )
            if ( line[c] == ',' )
                line[c] = ' ';

        sscanf( line, "%d %d %le\n", &row, &col, &val );

        row--;
        col--;

        if (( col >= start_idx ) && ( col < start_idx + my_nbr_cols ))
            Fill_Field_Date( row, col, val, lencol );
        // matrix is symmetric column
        // data structure must carry full matrix
        if (row != col)
        {
            if (( row >= start_idx ) && ( row < start_idx + my_nbr_cols ))
                Fill_Field_Date( col, row, val, lencol );
        }
        else // row == col
            if (( col >= start_idx ) && ( col < ( start_idx + my_nbr_cols )))
                diagonal_elements[pos++] = true;
    }
}



template<> void
Matrix_Reader<double,RCV>::Fill_Lines
(   const int                   len_col,
    Compressed_Lines<double>*&  lines,
    const int                   col,
    int&                        pos)
{
    for ( int c = 0; c < len_col; c++, pos++)
    {
        lines->mtx_vals[col][c] = cols[pos].val;
        lines->col_idcs[col][c] = cols[pos].i;
    }
}


//============================================================================
//============================================================================
//=============== Template specifications for COMPLEX matrices ===============
//============================================================================
//============================================================================


template<> void
Matrix_Reader<COMPLEX,RCC>::Fill_Field_Date
(   const int       row,
    const int       col,
    const double    real,
    const double    imag,
    const double    cc_val,
    int&            inc)
{
    cols[inc].i = row;
    cols[inc].j = col;
    cols[inc].c.real = real;
    cols[inc].c.imag = imag * cc_val;
    inc++;
}



template<> void
Matrix_Reader<COMPLEX, RCC>::Data_To_Memory
(   FILE    *f )
{
    int     row,
            col,
            lencol = 0,
            pos = 0;
    double  real,
            imag,
            cc_val = 1.0;
    char    line[128];

    diagonal_elements = new bool[my_nbr_cols];
    memset(diagonal_elements, false, my_nbr_cols*sizeof(bool));

    // if hermitian conjugate complex value
    // must be set to -1
    if ( hermitian ) cc_val = -1.0;

    for (int i = 0; i < file_nnz; i++)
    {
        if ( !fgets(line,128,f) ) break;
        // Change ',' to ' '
        for ( int c = 0; line[c]; c++ )
            if ( line[c] == ',' )
                line[c] = ' ';

        sscanf( line, "%d %d %le %le\n", &row, &col, &real, &imag );

        row--;
        col--;

        if (( col >= start_idx ) && ( col < start_idx + my_nbr_cols ))
            Fill_Field_Date( row, col, real, imag, 1.0, lencol );
        // adjust symmetric/hermitian part
        if ( row != col )
        {
            if (( row >= start_idx ) && ( row < start_idx + my_nbr_cols ))
                Fill_Field_Date( col, row, real, imag, cc_val, lencol );
        }
        else // row == col
            if (( col >= start_idx ) && ( col < ( start_idx + my_nbr_cols )))
                diagonal_elements[pos++] = true;
    }
}



template<> void
Matrix_Reader<COMPLEX,RCC>::Fill_Lines
(   const int                   len_col,
    Compressed_Lines<COMPLEX>*& lines,
    const int                   col,
    int&                        pos)
{
    for ( int c = 0; c < len_col; c++, pos++)
    {
        lines->mtx_vals[col][c] = cols[pos].c;
        lines->col_idcs[col][c] = cols[pos].i;
    }
}
