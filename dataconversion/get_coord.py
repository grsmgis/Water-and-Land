import os
import numpy as np
import h5py
import sys
#change the region name to the name of the main: from get_required_info
Region_name = "LTTB"
#do not change
file_name = Region_name + "_coord.txt"

o_file =  open(file_name, 'w')

#change tot he path and name of matlab file
f = h5py.File('GSMNP_LTTB_DEM_LAT_LON_3km.mat','r')
data = f.get('LAT')
data1 = np.array(data)

print(data1.shape[1], file=o_file)
print(data1.shape[0], file=o_file)
for i in data1[1]:
    print(i, file=o_file)
data = f.get('LON')
data1 = np.array(data)
for i in data1:
    print(i[1], file=o_file)
data = f.get('DEM')
data1 = np.array(data)
for i in data1:
    for j in i:
        print(j, file=o_file)

o_file.close()