// data_exchange.h

#ifndef DATA_EXCHANGE_H
#define DATA_EXCHANGE_H

#include "com.h"
#include "proc_maps.h"
#include <iostream>
#include <cstring>

namespace master {

   class Message {

      public:

         template<class Type>
            void send_to_slave(
                  Type* src,
                  int   count,
                  int   proc_id_dst,
                  int   task_id_dst,
                  int   num_slave_procs,
                  int   num_tasks )
            {

               // unique tag for sending data
               int send_tag =
                  MASTER_DATA_EXCHANGE         +
                  num_slave_procs * num_tasks  +
                  proc_id_dst * num_tasks      +
                  task_id_dst;

               com::proc::Isend(
                     src,                   // buffer
                     count,                 // count
                     proc_id_dst,           // destination id
                     send_tag,              // tag
                     com::proc::Comm_world,
                     &request );
            }

         // extract_data function from Message class in master namespace
         template<class Data_type>
            void extract_data(
                  Data_type*            src,
                  const std::string&    filename,
                  int                   count,
                  com::proc::Comm       dex_comm )
            {

               Meta meta_data;

               meta_data.type_size = sizeof(Data_type);
               meta_data.count     = count;

               // data extraction does not terminate the data extraction task
               meta_data.terminate = false;

               com::proc::Request l_request;

               // send meta data
               com::proc::Isend(
                     &meta_data,  // buffer
                     1,           // count
                     0,           // destination id
                     MASTER_META, // tag
                     dex_comm,    // comm handle
                     &l_request );

               // wait for meta data to be sent
               com::proc::wait( &l_request );

               // send source data
               com::proc::Isend(
                     src,         // buffer
                     count,       // count
                     0,           // destination id
                     MASTER_DATA, // tag
                     dex_comm,    // comm handle
                     &l_request );

               // wait for data to be sent
               com::proc::wait( &l_request );
            }

      private:
         com::proc::Request request;
   };

   void finalize_extraction( com::proc::Comm dex_comm );
}

namespace slave {

   class Message {
      public:

         // receive_from_master function from Message class in slave namespace
         template<class Type>
            void receive_from_master(
                  Type* dst,
                  int   count,
                  int   proc_id_src,
                  int   task_id_src,
                  int   proc_id_dst,
                  int   task_id_dst,
                  int   num_procs,
                  int   num_tasks )
            {

               // unique tag for sending data
               int recv_tag =
                  MASTER_DATA_EXCHANGE     +
                  num_procs * num_tasks    +
                  proc_id_dst * num_tasks  +
                  task_id_dst;

               com::proc::Irecv(
                     dst,                   // buffer
                     count,                 // count
                     proc_id_src,           // source id
                     recv_tag,              // tag
                     com::proc::Comm_world,
                     &request );
            }

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
               int send_tag =
                  SLAVE_DATA_EXCHANGE      +
                  num_procs * num_tasks    +
                  proc_id_dst * num_tasks  +
                  task_id_dst;

               com::proc::Isend(
                     src,                   // buffer
                     count,                 // count
                     proc_id_dst,           // destination id
                     send_tag,              // tag
                     com::proc::Comm_world,
                     &request );
            }

         // receive_from_slave function from Message class in slave namespace
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
               int recv_tag =
                  SLAVE_DATA_EXCHANGE      +
                  num_procs * num_tasks    +
                  proc_id_dst * num_tasks  +
                  task_id_dst;

               com::proc::Irecv(
                     dst,                   // buffer
                     count,                 // count
                     proc_id_src,           // source id
                     recv_tag,              // tag
                     com::proc::Comm_world,
                     &request );
            }

         // wait function from Message class in slave namespace
         void wait( void )
         {
            // wait for data to be sent
            com::proc::wait( &request );
         }

         // extract_data function from Message class in slave namespace
         template<class Data_type>
            void extract_data(
                  Data_type*            src,
                  const std::string&    filename,
                  int                   count,
                  int                   proc_id,
                  int                   task_id,
                  com::proc::Comm       dex_comm )
            {
               Meta meta_data;

               meta_data.type_size = sizeof(Data_type);
               meta_data.count     = count;

               // data extraction does not terminate the data extraction task
               meta_data.terminate = false;

               // copy the contents of the filename to the meta-data message
               std::strcpy( meta_data.filename, filename.c_str() );

               com::proc::Request l_request;

               int meta_tag = SLAVE_META + task_id;
               int data_tag = SLAVE_DATA + task_id;

               // send meta data
               com::proc::Isend(
                     &meta_data,  // buffer
                     1,           // count
                     0,           // destination id
                     meta_tag,    // tag
                     dex_comm,    // comm handle
                     &l_request );

               // wait for meta data to be sent
               com::proc::wait( &l_request );

               // send source data
               com::proc::Isend(
                     src,         // buffer
                     count,       // count
                     0,           // destination id
                     data_tag,    // tag
                     dex_comm,    // comm handle
                     &l_request );

               // wait for data to be sent
               com::proc::wait( &l_request );
            }

      private:
         com::proc::Request request;
   };

   void finalize_extraction( com::proc::Comm dex_comm, int task_id );
}

#endif
