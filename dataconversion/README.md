# Date management

## Convet binary files to netcdf

### Initial setup:

1. Install netcdf libray for c:
    * Ubuntu:
    ```$ sudo apt-get install libnetcdf-dev libnetcdff-dev```

2. Make libfdr library:

    *```$ cd libfdr```

    *```$ make```

### make_nc.c

* This file requires two files for it to work properly, name_coord.txt and get_required_info.c

#### get_required_info

* Edit the file path in getrequired_info.c then compile and execute. There will now be a file called get_required_info.txt

* This is a c program that goes through a dectory and compiles a list of information and produces a text file called ```required_info.txt```
* Reuired_info.txt:
  * Main: name of the region and what the nc file will be named
  * total_variables: A printed list of all variables in the region
  * end_of_total_variables: signifies the end of the list
  * var: name of the variable followed by a list of file paths in order by date 
  * end_of_main: signifies the end of main

### Compile make_nc.c and get_required_info.c

* ```$ make``` command in the dataconversion will do this for you

#### get_coord.py

* this file generates (region)_coord.txt file that is needed
* change the file paths as needed by editing the python file and executing it
  * ```$ python get_coord.py```
* contents of the created file:
  * number of latitudes
  * number of longitudes
  * list of latitudes
  * list of longitudes
  * list of dem
