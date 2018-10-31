#ifndef COMM_H
#define COMM_H

#include <mpi.h>
#include <vector>

#define MAX_TAGS   200
#define MAX_STAGES 20
#define MAX_INTERCOMMS MAX_STAGES * (MAX_STAGES - 1)
#define INVALID_TAG -1

namespace comm {

struct CONFIG {
   int processes[MAX_STAGES];
   int numStages;
};

class COMM {

   public:
      COMM (int* argc, char*** argv, CONFIG& config);
     ~COMM (void);

      template<typename type> bool send_to_stage (type* data, int size, int stage, int recvStageRank, int tag);
      template<typename type> bool send_to_stage (type* data, int size, int stage, int tag) {
         return send_to_stage (data, size, stage, 0, tag);
      }

   private:
      MPI_Comm interComms[MAX_INTERCOMMS];

      int sendTags[MAX_TAGS];
      int recvTags[MAX_TAGS];

      std::vector< std::vector< std::vector<MPI_Request*> > > sendRequests;
      std::vector< std::vector< std::vector<MPI_Request*> > > recvRequests;

};

}

#endif
