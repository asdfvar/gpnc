#!/usr/bin/python

import struct

fid = open('../../main/run/master_filename', 'rb')
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
fid.close()

# meta data:
# com::proc::Comm, char filename[256], int type_size, int count, bool terminate
fid = open('../../main/run/master_filename.meta', 'rb')
fid.close()
