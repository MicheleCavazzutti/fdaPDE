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
#include "../Include/Timer.h"

//C/C++ includings
#include <iostream>
#include <sys/time.h>
#include <ctime>


//static definitions
double  Timer::dbg_start_timers[dbg_timers];
double  Timer::dbg_stop_timers[dbg_timers];
double  Timer::dbg_sum_timers[dbg_timers];


Timer::Timer
( )
{
    start_timer = 0.0;
    stop_timer  = 0.0;
    sum_time    = 0.0;
}



void
Timer::Start
( ENV_Handler& env_handler )
{
    env_handler.Get_Time( start_timer, tp );
}



void
Timer::Stop
( ENV_Handler& env_handler )
{
    double time_diff;

    env_handler.Get_Time( stop_timer, tp );
    time_diff = stop_timer - start_timer;
    sum_time += time_diff;
}



void
Timer::Report
(   ENV_Handler& env_handler )
{
    double  max;
    int     num_procs,
            my_id;

    env_handler.Get_Environment_Params( num_procs, my_id );
    env_handler.Time_Diff( max, sum_time );

    /*if( my_id == 0 )
    {
        std::cout.precision( 10 );
        std::cout << "  " << max << "\t[sec]" << std::endl;
    }
    */
    sum_time = 0.0;
}



void
Timer::Dbg_Start
(   int          id,
    ENV_Handler& env_handler )
{
    env_handler.Get_Time( start_timer,tp );
    dbg_start_timers[id] = start_timer;
}



void
Timer::Dbg_Stop
(   int          id,
    ENV_Handler& env_handler )
{
    double diff;

    env_handler.Get_Time( stop_timer,tp );
    dbg_stop_timers[id] = stop_timer;
    diff = dbg_stop_timers[id] - dbg_start_timers[id];
    dbg_sum_timers[id] += diff;
}



void
Timer::Dbg_Report
( ENV_Handler& env_handler ) const
{
    int     num_procs,
            my_id;
    double  *sum = NULL;

    env_handler.Get_Environment_Params( num_procs, my_id );

    sum = new double[num_procs];
    for( int id = 0; id < dbg_timers; id++ )
    {
        // Getting times of all operations from all pe's
        env_handler.Dbg_Timers( dbg_sum_timers[id], sum );

        /*if( my_id == 0 )
        {
            std::cout.precision( 10 );
            std::cout << "######## id: " << id << std::endl;
            for( int i = 0; i < num_procs; i++ )
                std::cout << "  processor "<< i
                          << ":  sum: "<< sum[i] << std::endl;
            std::cout << std::endl;
        }
        */
    }
    delete [] sum;
}
