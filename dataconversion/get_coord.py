import os
import numpy as np
import h5py
import sys

#add the name of the basins (only thing you have to change as long as the file names follow the same format, and pass the base path as argument)
baseFilePath = sys.argv[1]
basins = [ "LFBB", "PRB", "LPRB", "LTTB"]
try:  
    for Region_name in basins:
        file_name = Region_name + "_map_info.txt"
        f = h5py.File(baseFilePath + Region_name + '/GSMNP_'+Region_name+'_DEM_LAT_LON_3km.mat','r')
        f1 = h5py.File(baseFilePath + Region_name + '/GSMNP_'+Region_name+'_Region_BDY_Pix_3km.mat','r')

        o_file =  open(file_name, 'w')

        variables = f.items();

        #used to define the size of the varibles 
        print("define", file=o_file)
        print("lat-long", file=o_file)
        data = f.get('LAT')
        data = np.array(data)
        print(data.shape[1], file=o_file)
        print(data.shape[0], file=o_file)

        print("end_define", file=o_file)

        print('lat', file=o_file)
        data = f.get('LAT')
        data = np.array(data)
        for i in data[1]:
            print(i, file=o_file)

        print('lon', file=o_file)
        data = f.get('LON')
        data = np.array(data)
        for i in data:
            print(i[1], file=o_file)

        print("dem", file=o_file)
        data = f.get('DEM')
        data = np.array(data)
        for i in data:
            for j in i:
                print(j, file=o_file)

        print("bdy", file=o_file)
        data = f1.get("BDY")
        data = np.array(data)
        for i in data:
            for j in i:
                print(j, file=o_file)

        o_file.close()
except:
    print("Error occured, please check input arguments")