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
#include "../Include/Command_Parser.h"

//C/C++ includings
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <sstream>


Command_Parser::Command_Parser
(   ENV_Handler& env_handler )
{
  char output_file_init[]   = "precond.mtx";
  char solver_file_init[]  = "solution.mtx";
    
    //Set default values
    epsilon_param   = 0.4;
    updates_param   = 5;
    max_idcs_param  = 5;
    use_mean_param  = true;
    write_param     = 1;
    hash_param      = 6;
    alg_level       = 0;
    pattern_file    = NULL;
    matrix_file     = NULL;
    output_file     = &output_file_init[0];
    solver_param    = 1;
    solver_tol      = 1e-06;
    solver_maxit    = 1000;
    solver_file     = &solver_file_init[0];
    rhs_param       = 1;
    para_max_levels = 1;
    para_threshold  = 0.1;
    para_filter     = 0.05;

    env_handler.Set_Help( known_params, options );
}



void
Command_Parser::Read_Parameters
(   int          argc,
    char         *argv[],
    ENV_Handler& env_handler)
{
    static  char mtx_tmp[256],
                 pattern_tmp[256];

    // User needs help?
    if (( argc > 1 ) && ( strncmp( argv[1], "-h", 2 ) == 0 ))
        throw std::runtime_error(
            "\n\t========================== HELP "
                "===========================\n"
            "\n\tinput parameters (first and second are mandatory):\n\n"
            "\tsystem matrix:\n"
            "\t\tmatrix file in MatrixMarket format\n\n"
            "\tstart pattern:\n"
            "\t\t-pattern file in MatrixMarket pattern format or \n"
            "\t\t-diag for diagonal pattern without file or\n"
            "\t\t-L for lower triangular pattern of system without file\n\n\n"
            "\toptions (any order, always -code number/path, e.g. -ep 0.15):\n\n"
            "\t" + options + "\n"
            "\n\t================================"
                "===========================\n");

    //User inputed less than 3 Parameter -> help required
    if ( argc < 3 )
        throw std::runtime_error(
            "\n\tusage: ./fspai matrix.mtx [pattern.mtx |-diag| -L]"
            " options\n\n"
            "\t\tor: ./fspai -h[elp] for help on input parameters\n\n");

    //Setting matrix file
    strcpy( mtx_tmp,argv[1] );
    matrix_file = mtx_tmp;

    // Setting start pattern parameters
    if ( argv[2][0] != '-' )
    {
        pattern_param = 0;
        strcpy(pattern_tmp,argv[2]);
        pattern_file = pattern_tmp;
    }
    else
    {
        if ( strncmp( argv[2],"-diag",5 ) == 0 )
            pattern_param = 1;
        else if ( strncmp( argv[2],"-L",2 ) == 0 )
            pattern_param = 2;
        else
            throw std::runtime_error(
                "\n\t\tIllegal pattern parameter: "
                    + std::string(argv[2]) + "\n"
                "\t\tAllowed are: \n"
                "\t\t\t-arbitrary pattern file (no number required)\n"
                "\t\t\t-diag for diagonal pattern (no file required)\n"
                "\t\t\t-L for lower triangular pattern of the system\n"
                "\t\t\t   matrix(no file required)\n"
                "\n\t\tUse -h(elp) for details.\n");
    }
    Fill_Param_Map( argc,argv );
    Set_Params();
    Check_Params(env_handler);
}



void
Command_Parser::Fill_Param_Map
(   int     argc,
    char    *argv[] )
{
    for ( int i = 3; i < argc; i++ )
    {
        if ( i + 1 < argc && argv[i+1][0] != '-' )
        {
            std::string key( ++argv[i] );
            if ( known_params.find( key ) == std::string::npos )
                throw std::runtime_error( "\n\t\tIllegal parameter: "+key+"\n" );
            std::string value( argv[i+1] );
            parameters.insert( std::pair<std::string,
                               std::string>( key, value ) );
        }
    }
}



void
Command_Parser::Set_Params
( )
{
    parameters.Get_Arg_Typed( "ep", epsilon_param );
    parameters.Get_Arg_Typed( "ns", updates_param );
    parameters.Get_Arg_Typed( "mn", max_idcs_param );
    parameters.Get_Arg_Typed( "um", use_mean_param );
    parameters.Get_Arg_Typed( "wp", write_param );
    parameters.Get_Arg_Typed( "hs", hash_param );
    parameters.Get_Arg_Typed( "out", output_file );
    parameters.Get_Arg_Typed( "sol", solver_param );
    parameters.Get_Arg_Typed( "sol_tol", solver_tol );
    parameters.Get_Arg_Typed( "sol_maxit", solver_maxit );
    parameters.Get_Arg_Typed( "sol_out", solver_file );
    parameters.Get_Arg_Typed( "rhs", rhs_param );
    parameters.Get_Arg_Typed( "para_levels", para_max_levels );
    parameters.Get_Arg_Typed( "para_thresh", para_threshold );
    parameters.Get_Arg_Typed( "para_filter", para_filter );
}



void
Command_Parser::Check_Params (ENV_Handler& env_handler )
{
    std::stringstream   out_str;

    // check for correct parameter values
    // Parameters are automatically set to specified values:
    // If parameter value is out of scope, the default value is used.

    //check for correct hash table size
    //if not, abort program from here
    if (hash_param < 0 || hash_param > 6)
    {
      out_str << hash_param;
      throw std::runtime_error(
          "\n\t\tIllegal hash table size parameter: "
              + out_str.str() + "\n"
          "\t\tAllowed are:\n"
          "\t\t\t0: not using hash table (very slow)\n"
          "\t\t\t1: size is 101\n"
          "\t\t\t2: size is 503\n"
          "\t\t\t3: size is 2503\n"
          "\t\t\t4: size is 12503\n"
          "\t\t\t5: size is 62501\n"
          "\t\t\t6: size is 104743 (default)\n");
    }

    // hash size should be prime to 5 because of
    // linear rehashing
    if(hash_param == 0)      hash_param = 0;
    else if(hash_param == 1) hash_param = 101;
    else if(hash_param == 2) hash_param = 503;
    else if(hash_param == 3) hash_param = 2503;
    else if(hash_param == 4) hash_param = 12503;
    else if(hash_param == 5) hash_param = 62501;
    else if(hash_param == 6) hash_param = 104743;

    // check for correct solver parameter
    if(solver_param < 0 || solver_param > 3)
    {
        out_str << solver_param;
        throw std::runtime_error(
            "\n\t\tIllegal solver parameter: "
            + out_str.str() + "\n"
            "\t\tAllowed are:\n"
            "\t\t\t0: not using PCG solver\n"
            "\t\t\t1: use PCG solver with computed FSPAI\n"
            "\t\t\t2: unpreconditioned CG solver and PCG with FSPAI\n"
            "\t\t\t3: use PCG with FSPAI and PCG with PARASAILS\n");
    }

    // Check for correct solver parameter in sequential environment,
    // where -sol has to be 0-2
    env_handler.Check_Solver_Param(solver_param);

    // check for correct rhs parameter
    if(rhs_param < 0 || rhs_param > 1)
    {
        out_str << rhs_param;
        throw std::runtime_error(
            "\n\t\tIllegal right-hand side parameter: "
            + out_str.str() + "\n"
            "\t\tAllowed are:\n"
            "\t\t\t0: use random vector as rhs\n"
            "\t\t\t1: use ones vector as rhs\n");
    }
}
