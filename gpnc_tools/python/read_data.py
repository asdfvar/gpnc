#!/usr/bin/python

import struct

fid = open('../../main/run/master_data', 'rb')
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
data = struct.unpack('i', fid.read(4))
print data
