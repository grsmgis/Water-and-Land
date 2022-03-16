#include<stdlib.h>
#include<netcdf.h>
#include<string.h>
#include <stdio.h>
#include "fields.h"
#define NDIMS 3
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(2);}
#define DEGREES_EAST "degrees_east"
#define DEGREES_NORTH "degrees_north"
typedef struct write_nc write_nc;
struct write_nc{

    int ncid;
    int retval;
    int dimids[NDIMS];
    int Nlong;
    int Nlat; 
    int Ntime;
    int Nvars;
    int current_time;
    int lat_id, long_id, time_id, dem_id, lat_varid, lon_varid, dem_varid;
    int vars_id[100];
    char * vars[100];
    float longs[100];
    float lats[100];
    float dems[100][100];
    size_t count[NDIMS];
    size_t start[NDIMS];

};
//constructor, given a file name of the coordinats, it will set the according variables
void set_variables(int Ntime1, char * coord_file, IS is, write_nc * w_n){
    FILE * fid = fopen(coord_file, "r");
    
    if (fid == NULL) {
        perror("Failed: opening coord_file");
        exit(1);
    }

    if ( fscanf(fid, "%i", &w_n->Nlat) ==0 || fscanf(fid, "%i", &w_n->Nlong)==0){
        perror("file not in correct format");
        exit(1);
    }
    
    for(int i = 0; i < w_n->Nlat; i++){
        fscanf(fid, "%f",&w_n->lats[i]);
    }
    for(int i = 0; i < w_n->Nlong; i++){
        fscanf(fid, "%f",&w_n->longs[i]);
    }
    for(int i = 0; i < w_n->Nlat; i++){
        for(int j = 0; j < w_n->Nlong; j++ ){
            fscanf(fid, "%f", &w_n->dems[i][j]);
        }
    }
    fclose(fid);
        
        
    w_n->Ntime = Ntime1;
    //get the variables
    int correct_label = 0;
    w_n->Nvars = 0;
    while(get_line(is) >= 0){
        // printf("this is fields 0 %s\n", is->fields[0]);
        if(correct_label != 1){
            if(strcmp(is->fields[0], "total_variables:") == 0 ){
                correct_label = 1;
                continue;
            }
            else{
                perror("no total variables lable\n");
                exit(1);
            }
        }
        
        if(strcmp(is->fields[0], "end_of_total_variables") == 0 ) return;
        
        w_n->vars[w_n->Nvars] = strdup(is->fields[0]);
        w_n->Nvars++;
    }
    w_n->current_time = 0;
    w_n->count[0] = 1;
    w_n->count[1] = w_n->Nlong;
    w_n->count[2] = w_n->Nlat;
    w_n->start[1] = 0;
    w_n->start[2] = 0;
}

//creating t
void create_and_define(char * nc_name, write_nc * w_n){
    printf("name of nc file: %s\n", nc_name);
    if ((w_n->retval = nc_create(nc_name, NC_CLOBBER, &w_n->ncid)))
        ERR(w_n->retval);
    /* Define the dimensions. NetCDF will hand back an ID for each. */
    if ((w_n->retval = nc_def_dim(w_n->ncid, "longitude", w_n->Nlong, &w_n->long_id)))
        ERR(w_n->retval);
    
    if ((w_n->retval = nc_def_dim(w_n->ncid, "latitude", w_n->Nlat, &w_n->lat_id)))
        ERR(w_n->retval);
    
    if ((w_n->retval = nc_def_dim(w_n->ncid, "time", NC_UNLIMITED, &w_n->time_id)))
        ERR(w_n->retval);

    /*Define each the coordinate variables*/
    if ((w_n->retval = nc_def_var(w_n->ncid, "longitude", NC_FLOAT, 1, &w_n->long_id, &w_n->lon_varid)))
        ERR(w_n->retval);
    
    if ((w_n->retval = nc_def_var(w_n->ncid, "latitude", NC_FLOAT, 1, &w_n->lat_id, &w_n->lat_varid)))
        ERR(w_n->retval);
    
    /*Define DEM variable*/
    if ((w_n->retval = nc_def_var(w_n->ncid, "Dem", NC_FLOAT, 2, &w_n->dem_id, &w_n->dem_varid)))
        ERR(w_n->retval);
    
    /* Assign units attributes to coordinate variables. */
    if ((w_n->retval = nc_put_att_text(w_n->ncid, w_n->lat_varid, "units", strlen(DEGREES_NORTH), DEGREES_NORTH)))
        ERR(w_n->retval);
    if ((w_n->retval = nc_put_att_text(w_n->ncid, w_n->lon_varid, "units", strlen(DEGREES_EAST), DEGREES_EAST)))
        ERR(w_n->retval);
   
    /* The dimids array is used to pass the dimids of the dimensions of
    the netCDF variables.*/
    w_n->dimids[0] = w_n->time_id;
    w_n->dimids[1] = w_n->long_id;
    w_n->dimids[2] = w_n->lat_id;

    //define the variables
    for(int i = 0; i < w_n->Nvars; i++){
        int var_id;
        if ((w_n->retval = nc_def_var(w_n->ncid, w_n->vars[i], NC_FLOAT, NDIMS, w_n->dimids, &var_id)))
            ERR(w_n->retval);
        w_n->vars_id[i] = var_id;
    }
  
                
    /* End define mode. */
    if ((w_n->retval = nc_enddef(w_n->ncid)))
    ERR(w_n->retval);
    
}

void write_coords( write_nc * w_n){
    /* Write the coordinate variable data. This will put the latitudes
    and longitudes of our data grid into the netCDF file. */
    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->lat_id, &w_n->lats[0])))
    ERR(w_n->retval);
    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->long_id, &w_n->longs[0])))
    ERR(w_n->retval);

    float t_dem[w_n->Nlat][w_n->Nlong];
    for(int i = 0; i < w_n->Nlat; i++){
        for(int j = 0; j < w_n->Nlong; j++ ){
            t_dem[i][j] = w_n->dems[i][j];
        }
    }
    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->dem_varid, &t_dem[0][0])))
    ERR(w_n->retval);

}
//read from a input file and write that to the netcdf (file should contain 24 hr data)
void read_file_write(char * fname, int var_offset, write_nc * w_n ){
    // printf("writing %i", var_offset);
    float data_out[w_n->Nlong][w_n->Nlat];
    FILE *ptr = fopen(fname, "rb");
    if (ptr == NULL) {
        perror("Failed: opening ptr:");
        printf("%s", fname);
        exit(1);
    }
    char bytes[4];
    int ctime = 24 + w_n->current_time;
    for(int f =0; w_n->current_time < ctime && w_n->current_time < w_n->Ntime; f++, w_n->current_time++){
        for(int lo = 0; lo < w_n->Nlong; lo++){
            for(int la = 0; la < w_n->Nlat; la++){
                if(fread(bytes, 4, 1, ptr)!= 0){
                    float a = *(float*)&bytes;
                    data_out[lo][la] = a;
                }
            }
        }
        w_n->start[0] = w_n->current_time;
        int var_id = w_n->vars_id[var_offset];
        if ((w_n->retval = nc_put_vara_float(w_n->ncid, var_id, w_n->start, w_n->count, &data_out[0][0])))
            ERR(w_n->retval);
    }
    fclose(ptr);
}
//freeing any memeory allocated
void exiting( write_nc * w_n){
    printf("done writing file\n");
    for(int i = 0; i < w_n->Nvars; i++){
        free (w_n->vars[i]);
    }
    //close the ncid
    if ((w_n->retval = nc_close(w_n->ncid)))
       ERR(w_n->retval);
}
int main(){
    // // the 24 is the number of hours in a day, the 1248 is the number of files there are 
    int TNtime = 24 * 1248;

    IS is;
    write_nc * w_n; 
    is = new_inputstruct("required_info.txt");
    int correct = 0;
    int current_time = 0;
    int var_offset = 0;
    while(get_line(is) >= 0) {
        if(strcmp(is->fields[0], "Main:") == 0){
            // if(strcmp(is->fields[1], "PRB") == 0){
                //get the name of the coordinate for the region
                char tmp[1000];
                char main_name[1000];
                strcpy(main_name, is->fields[1]);
                strcpy(tmp, main_name);
                strcat(tmp,"_coord.txt\0");
                
                //declare write_nc struct object, get the coordinats from the correct file
                //get the variable names
                w_n = (write_nc *) malloc(sizeof(write_nc));
                set_variables(TNtime, tmp, is, w_n);
                

                strcpy(tmp, main_name);
                strcat(tmp,".nc\0");
                //create the nc file and define all of the variables and exit define mode
                create_and_define(tmp, w_n);

                //write out the coordinate variable data
                write_coords(w_n);
                correct =1;
            // }
            // else{
            //     correct = 0;
            // }
            continue;
        }
        else if(strcmp(is->fields[0], "end_of_main") == 0){
            exiting(w_n);
            free(w_n);
            correct = 0;
            var_offset = 0;
            continue;
        }
        if(correct){
            if(strcmp(is->fields[0], "var:") == 0) {
                
                
                w_n->count[0] = 1;
                w_n->count[1] = w_n->Nlong;
                w_n->count[2] = w_n->Nlat;
                w_n->start[1] = 0;
                w_n->start[2] = 0;
                w_n->current_time = 0;
                for(; var_offset < w_n->Nvars; var_offset++){
                    if(strcmp(is->fields[1], w_n->vars[var_offset])==0){
                        break;
                    }
                }
                // for(int i = 0; i < w_n->Nvars; i++){
                //     printf( "s var:  %s\n\n", w_n->vars[i]);
                // }
                printf( "writing var:  %s\n\n", w_n->vars[var_offset]);
                continue; 
            }
            char * fname = strdup(is->fields[0]);
            read_file_write(fname, var_offset, w_n);
            free(fname);

        }
    }
    
    
    jettison_inputstruct(is);
    
    printf("******** SUCCESS writing all NC files!********\n");
    return 0;

}
