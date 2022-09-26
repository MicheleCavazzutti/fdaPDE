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

// file includings
#include "../Include/Param_Map.h"


template<>          int
Param_Map::Converter<int>
(   std::string in,
    int&        out )
{
    out = atoi( in.c_str() );
    return 0;
}


template<>          int
Param_Map::Converter<bool>
(   std::string in,
    bool&       out )
{
    out = atoi( in.c_str() );
    return 0;
}


template<>          int
Param_Map::Converter<double>
(   std::string in,
    double&     out )
{
    std::istringstream is( in );
    is >> out;
    return 0;
}


template<>          int
Param_Map::Converter<char*>
(   std::string in,
    char*&      out )
{
    static char out_mem[256];
    strcpy( out_mem, in.c_str() );
    out = out_mem;
    return 0;
}
