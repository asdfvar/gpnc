#ifndef PROC_MAPS_H
#define PROC_MAPS_H

#include "com.h"

#define DATA_EXTRACTION_GROUP 0
#define MASTER_GROUP          1
#define SLAVE_GROUP           2

enum Proc_tags {
   MASTER_DATA_EXT, // inter-comm creation for master process
   MASTER_META,
   SLAVE_DATA_EXT   // inter-comm creation for slave process
};

typedef struct {
   com::proc::Comm dex_comm; // communication handle to the data extraction
   int             test;
   bool            finished;
} Meta;

#endif
