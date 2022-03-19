# Date management
## Convet binary files to netcdf

### Initial setup:

1. Install netcdf libray for c:
    * Ubuntu:
    ```$ sudo apt-get install libnetcdf-dev libnetcdff-dev```
    
2. Make libfdr library:
    *```$ cd libfdr ```
    *```$ make ```

### Compile make_nc.c and get_required_info.c
- make command in the dataconversion will do this for you

### get_required_info:
-   The command will only work if the path given is in the order of :
    >region 
    > > variable
    > > >fdates 
    -   ```
    - 