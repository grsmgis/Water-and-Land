# Date management

## Convet binary files to netcdf

### Initial setup

1. Install netcdf libray for c:
    * Ubuntu:
    ```$ sudo apt-get install libnetcdf-dev libnetcdff-dev```

2. Make libfdr library:

    * ```$ cd libfdr```

    * ```$ make```
3. Compile make_nc.c and get_input_info.c

* ```$ make``` command in the dataconversion directory will do this for you

### make_nc.c

* This file requires two files for it to work properly, name_coord.txt and required_info.txt
* optional varnames.txt

### required_info.txt

* For Input Variables:
  * get_input_info.c
    * After files have been compiled throgh the make command, send the file path the only command line argument:
      * ```$ ./get_input_info path/to/rawfiles```
    * This is a c program that goes through a dectory and compiles a list of information and produces a text file called
      * ```required_info.txt```

* For Output Variables:
  * get_output_info.py
    * In the file the import files that refrence the variabls are in the dictionary and you can add to them as you see fit.
    * You have to give the correct path to the starting directory as a command line argument:
      * ```$ python3 get_ouypuy_info path/to/raw/output/folder/```

* Required_info.txt:
  * Main: name of the region and what the nc file will be named
  * total_variables: A printed list of all variables in the region
  * end_of_total_variables: signifies the end of the list
  * var: name of the variable followed by a list of file paths in order by date
  * end_of_main: signifies the end of main

### get_coord.py

* Files are needed in both Input and Output file generation.
* Uses the dem and region mat files for each region.
* This file generates (region)_coord.txt file(s) that are needed
* Pass the base_path to the GSMNP_Geo_Info directory as a command line argument:
  * ```$ python get_coord.py path/to/GSMNP_Geo_Info/```
* Contents of the created file:
  * number of latitudes
  * number of longitudes
  * list of latitudes
  * list of longitudes
  * list of dem
  * list of bdy

### varnames.txt

* Optional file that will be used to translate variable names to be what you want with the unit type you want. To make your own or add variables follow template:
  * Current Name of Variable
  * New Name of variable
  * Unit type of the variable (```NONE``` if no type is needed)

### make_nc

* After all previous steps are followed running ```$ ./make_nc``` will create all of the nc files named (mainvar).nc, with mainvar corresponding to the main label/region name in get_required_info.txt
