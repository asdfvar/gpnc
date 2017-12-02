// topology.h

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

namespace master {
   typedef struct {
      int type;
   } TOPOLOGY;
}

namespace slave {
   typedef struct {
      int type;
      int proc_id;
      int task_id;
      int num_procs;
      int num_tasks;
   } TOPOLOGY;
}

#endif
