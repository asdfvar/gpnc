// topology.h

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

enum topoType { NONE };

namespace master {
   typedef struct {
      topoType type;
   } TOPOLOGY;
}

namespace slave {
   typedef struct {
      topoType type;
      int      proc_id;
      int      task_id;
      int      num_procs;
      int      num_tasks;
   } TOPOLOGY;
}

#endif
