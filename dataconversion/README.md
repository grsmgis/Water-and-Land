# Date management

## Convet binary files to netcdf

### Initial setup

1. Install netcdf libray for c:
    * Ubuntu:
    ```$ sudo apt-get install libnetcdf-dev libnetcdff-dev```

2. Make libfdr library:

    * ```$ cd libfdr```

    * ```$ make```

### make_nc.c

* This file requires two files for it to work properly, name_coord.txt and required_info.txt
* optional varnames.txt

### required_info.txt

* For Input Variables:
  * get_required_info.c
    * Edit the file path in getrequired_info.c then compile and execute. There will now be a file called get_required_info.txt
    * This is a c program that goes through a dectory and compiles a list of information and produces a text file called ```required_info.txt```

* For Outpur Variables:
  * get_output_info.py
    * In the file the import files that refrence the variabls are in the dictionary and you can add to them as you see fit.
    * You have to give the correct path to the starting directory (for all file names this sould work but for the PRB file it is not named the same as other files, so I would suggest putting that in its own directory and running that region individually with the ``main = foldersplit("_")[0][3:]`` being changed to ``main = foldersplit("_")[0][4:]``

* Required_info.txt:
  * Main: name of the region and what the nc file will be named
  * total_variables: A printed list of all variables in the region
  * end_of_total_variables: signifies the end of the list
  * var: name of the variable followed by a list of file paths in order by date
  * end_of_main: signifies the end of main

### get_coord.py

* Files are needed in both Input and Output file generation.
* Uses the dem and region mat files for each region.
* This file generates (region)_coord.txt file that is needed
* Change the file paths as needed by editing the python file and executing it
  * ```$ python get_coord.py```
* Contents of the created file:
  * number of latitudes
  * number of longitudes
  * list of latitudes
  * list of longitudes
  * list of dem
  * list of bdy

### Compile make_nc.c and get_required_info.c

* ```$ make``` command in the dataconversion will do this for you

### make_nc

* After all previous steps are followed running ```$ ./make_nc``` will create all of the nc files named (mainvar).nc, with mainvar corresponding to the main label/region name in get_required_info.txt
