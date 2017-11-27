// data_exchange.h

#ifndef DATA_EXCHANGE_H
#define DATA_EXCHANGE_H

#include "com.h"
#include <iostream>

namespace slave {

   template<class Type>
      void send_to_slave(
            Type* src,
            int   count,
            int   proc_id_dst,
            int   task_id_dst,
            int   num_procs,
            int   num_tasks )
      {

         // unique tag for sending data
         int send_tag = SLAVE_DATA_EXCHANGE      +
                        num_procs * num_tasks    +
                        proc_id_dst * num_tasks  +
                        task_id_dst;

         com::proc::Request request;

         com::proc::Isend(
               src,                   // buffer
               count,                 // count
               proc_id_dst,           // destination id
               send_tag,              // tag
               com::proc::Comm_world,
               &request );

         // wait for source data to be sent
         com::proc::wait( &request );
      }

   template<class Type>
      void receive_from_slave(
            Type* dst,
            int   count,
            int   proc_id_src,
            int   task_id_src,
            int   proc_id_dst,
            int   task_id_dst,
            int   num_procs,
            int   num_tasks )
      {

         // unique tag for receiving data
         int recv_tag = SLAVE_DATA_EXCHANGE      +
                        num_procs * num_tasks    +
                        proc_id_dst * num_tasks  +
                        task_id_dst;

         com::proc::Request request;

         com::proc::Irecv(
               dst,                   // buffer
               count,                 // count
               proc_id_src,           // source id
               recv_tag,              // tag
               com::proc::Comm_world,
               &request );

         // wait for meta data to be sent
         com::proc::wait( &request );
      }
}

#endif
