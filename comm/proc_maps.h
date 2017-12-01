#ifndef PROC_MAPS_H
#define PROC_MAPS_H

#include "com.h"

#define DATA_EXTRACTION_GROUP 0
#define MASTER_GROUP          1
#define SLAVE_GROUP           2

#define TAG_OFFSET 1000

enum Proc_tags {
   MASTER_DATA_EXT, // inter-comm creation for master process
   MASTER_META          = MASTER_DATA_EXT     + TAG_OFFSET, // meta data for master
   MASTER_DATA          = MASTER_META         + TAG_OFFSET, // data for master
   SLAVE_META           = MASTER_DATA         + TAG_OFFSET, // meta data for slave
   SLAVE_DATA           = SLAVE_META          + TAG_OFFSET, // data for slave
   SLAVE_DATA_EXCHANGE  = SLAVE_DATA          + TAG_OFFSET,
   MASTER_DATA_EXCHANGE = SLAVE_DATA_EXCHANGE + TAG_OFFSET,
};

typedef struct {
   com::proc::Comm dex_comm;  // communication handle to the data extraction
   char            filename[256]; // filename
   int             type_size; // size of data type
   int             count;     // data array size
   bool            terminate; // termination flag
} Meta;

#endif
