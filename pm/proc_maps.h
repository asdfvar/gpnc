#ifndef PROC_MAPS_H
#define PROC_MAPS_H

#define DATA_EXTRACTION_GROUP 0
#define MASTER_GROUP          1
#define SLAVE_GROUP           2

enum Proc_tags {
   MASTER_DATA_EXT, // inter-comm creation for master process
   SLAVE_DATA_EXT   // inter-comm creation for slave process
};

#endif
