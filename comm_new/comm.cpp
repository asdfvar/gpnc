#include "comm.h"
#include <iostream>

namespace comm {

/*
** constructor name: COMM
*/
COMM::COMM (int* argc, char*** argv, CONFIG& config)
{

   int required = MPI_THREAD_MULTIPLE;
   int provided;

   MPI_Init_thread (argc, argv, required, &provided);

   if (provided != required) {
      if (provided == MPI_THREAD_SINGLE) {
         // only one thread will execute
         std::cout << "provided level of MPI support is MPI_THREAD_SINGLE" << std::endl;
      }
      else if (provided == MPI_THREAD_FUNNELED) {
         // only the main thread will make MPI calls
         std::cout << "provided level of MPI support is MPI_THREAD_FUNNELED" << std::endl;
      }
      else if (provided == MPI_THREAD_SERIALIZED) {
         // any thread can make MPI calls but only serialized
         std::cout << "provided level of MPI support is MPI_THREAD_SERIALIZED" << std::endl;
      }
      else if (provided == MPI_THREAD_MULTIPLE) {
         // any thread can make MPI calls at any time
         std::cout << "provided level of MPI support is MPI_THREAD_MULTIPLE" << std::endl;
      }
   }

   for (int index = 0; index < MAX_TAGS; index++) sendTags[index] = INVALID_TAG;
   for (int index = 0; index < MAX_TAGS; index++) recvTags[index] = INVALID_TAG;
   for (int index = 0; index < MAX_INTERCOMMS; index++) interComms[index] = MPI_COMM_NULL;

   for (int stage = 0; stage < config.numStages; stage++) {
      sendRequests.push_back (std::vector< std::vector< MPI_Request* > >() );
   }
}

/*
** destructor name: COMM
*/
COMM::~COMM (void) {

   while (!sendRequests.empty()) {
      while (!sendRequests.back().empty()) {
         while (!sendRequests.back().back().empty()) {
            sendRequests.back().back().pop_back();
         }
         sendRequests.back().pop_back();
      }
      sendRequests.pop_back();
   }

}

} // namespace comm
