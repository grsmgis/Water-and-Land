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
    int Nlong, Nlat, Ntime, Nvars, Nindnanregion, Nindregion, Nindregionbdy;
    int current_time;
    int lat_id, long_id, time_id, dem_id, indnanregion_id, indregion_id, indregionbdy_id, bdy_id,
    lat_varid, long_varid, dem_varid, indnanregion_varid, indregion_varid, indregionbdy_varid, bdy_varid;
    int vars_id[100];
    char * vars[100];
    size_t count[NDIMS];
    size_t start[NDIMS];

};
//for renameing variables optional
typedef struct change_varname change_varname ;
struct change_varname{
    char * original;
    char * name;
    char * description;
};

//constructor, given a file name of the coordinats, it will set the according variables
void set_variables(int Ntime1, char * map_info, IS is, write_nc * w_n){
    IS ins = new_inputstruct(map_info);
        get_line(ins);
        if(strcmp(ins->fields[0], "define") == 0){
            while(get_line(ins) >= 0){
                if(strcmp(ins->fields[0], "end_define") ==0){
                    break;
                }
                else if(strcmp(ins->fields[0], "lat-long") ==0){
                    get_line(ins);
                    w_n->Nlat = atoi(ins->fields[0]);
                    get_line(ins);
                    w_n->Nlong = atoi(ins->fields[0]);
                }
            }
        }
    jettison_inputstruct(ins);
        
        
    w_n->Ntime = Ntime1;
    //get the variables
    int correct_label = 0;
    w_n->Nvars = 0;
    while(get_line(is) >= 0){
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

//creating all the needed variables and defining them
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
    if ((w_n->retval = nc_def_var(w_n->ncid, "longitude", NC_FLOAT, 1, &w_n->long_id, &w_n->long_varid)))
        ERR(w_n->retval);
    
    if ((w_n->retval = nc_def_var(w_n->ncid, "latitude", NC_FLOAT, 1, &w_n->lat_id, &w_n->lat_varid)))
        ERR(w_n->retval);
    
    /*Define region variables*/
    if ((w_n->retval = nc_def_var(w_n->ncid, "dem", NC_FLOAT, 2, &w_n->dem_id, &w_n->dem_varid)))
        ERR(w_n->retval);
    if ((w_n->retval = nc_def_var(w_n->ncid, "bdy", NC_FLOAT, 2, &w_n->bdy_id, &w_n->bdy_varid)))
        ERR(w_n->retval);

    /* Assign units attributes to coordinate variables. */
    if ((w_n->retval = nc_put_att_text(w_n->ncid, w_n->lat_varid, "units", strlen(DEGREES_NORTH), DEGREES_NORTH)))
        ERR(w_n->retval);
    if ((w_n->retval = nc_put_att_text(w_n->ncid, w_n->long_varid, "units", strlen(DEGREES_EAST), DEGREES_EAST)))
        ERR(w_n->retval);
   
    /* The dimids array is used to pass the dimids of the dimensions of
    the netCDF variables.*/
    w_n->dimids[0] = w_n->time_id;
    w_n->dimids[1] = w_n->long_id;
    w_n->dimids[2] = w_n->lat_id;
    

    FILE * varnames;
    //see if you need to rename the variables (optional)
    if(varnames = fopen("varnames.txt", "r")){
        fclose(varnames);
        IS ins;
        change_varname *cvar[w_n->Nvars];
        for(int i =0; i < w_n->Nvars; i++){
            cvar[i] = (change_varname *) malloc(sizeof(change_varname));
            cvar[i]->original = strdup(w_n->vars[i]);
            cvar[i]->name = NULL;
            cvar[i]->description = NULL;
        }
        ins = new_inputstruct("varnames.txt");
        while(get_line(ins) >= 0) {
            for(int i = 0; i < w_n->Nvars; i++){
                if(strcmp(ins->fields[0], w_n->vars[i]) == 0){
                    get_line(ins);
                    cvar[i]->name = strdup(ins->fields[0]);
                    get_line(ins);
                    cvar[i]->description = strdup(ins->fields[0]);
                }
            }
        }
        jettison_inputstruct(ins);
        for(int i = 0; i < w_n->Nvars; i++){
            int var_id;
            if(cvar[i]->name== NULL){
                if ((w_n->retval = nc_def_var(w_n->ncid, cvar[i]->original, NC_FLOAT, NDIMS, w_n->dimids, &var_id)))
                    ERR(w_n->retval);
                w_n->vars_id[i] = var_id;
                continue;
            }
            else{
                if ((w_n->retval = nc_def_var(w_n->ncid, cvar[i]->name, NC_FLOAT, NDIMS, w_n->dimids, &var_id)))
                    ERR(w_n->retval);
            }
            if(strcmp(cvar[i]->description, "NONE") ==0){
                w_n->vars_id[i] = var_id;
                continue;
            }

            w_n->vars_id[i] = var_id;
            if ((w_n->retval = nc_put_att_text(w_n->ncid, w_n->vars_id[i], "units", strlen(cvar[i]->description), cvar[i]->description)))
                ERR(w_n->retval);
        }

        //free memory:
        for(int i = 0; i < w_n->Nvars; i++){
            free(cvar[i]->original);
            if(cvar[i]->name != NULL) free(cvar[i]->name);
            if(cvar[i]->description!= NULL) free(cvar[i]->description);
            free(cvar[i]);
        }

    }
    //define the variables
    else{
        for(int i = 0; i < w_n->Nvars; i++){
            int var_id;
            if ((w_n->retval = nc_def_var(w_n->ncid, w_n->vars[i], NC_FLOAT, NDIMS, w_n->dimids, &var_id)))
                ERR(w_n->retval);
            w_n->vars_id[i] = var_id;
        }
    }
                
    /* End define mode. */
    if ((w_n->retval = nc_enddef(w_n->ncid)))
        ERR(w_n->retval);
    
}

void write_region_info( char * map_info, write_nc * w_n){

    IS ins = new_inputstruct(map_info);
        while(get_line(ins)>=0){
            if(strcmp(ins->fields[0], "end_define") ==0){
                    break;
                }
        }
        if(strcmp(ins->fields[0], "end_define") == 0){
            while(get_line(ins) >= 0){
                if(strcmp(ins->fields[0], "lat") ==0){
                    float lats[w_n->Nlat];     
                    for(int i = 0; i < w_n->Nlat; i++){
                        get_line(ins);
                        lats[i] = atof(ins->fields[0]);
                    }
                    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->lat_id, &lats[0])))
                        ERR(w_n->retval);
                }
                else if(strcmp(ins->fields[0], "lon") ==0){
                    float longs[w_n->Nlong];     
                    for(int i = 0; i < w_n->Nlong; i++){
                        get_line(ins);
                        longs[i] = atof(ins->fields[0]);
                    }
                    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->long_id, &longs[0])))
                        ERR(w_n->retval);
                }
                else if(strcmp(ins->fields[0], "dem") ==0){
                    float dem[w_n->Nlat][w_n->Nlong];
                    for(int i = 0; i < w_n->Nlat; i++){
                        for(int j = 0; j < w_n->Nlong; j++ ){
                            get_line(ins);
                            dem[i][j] = atof(ins->fields[0]);
                        }
                    }
                    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->dem_varid, &dem[0][0])))
                        ERR(w_n->retval);

                }
                else if(strcmp(ins->fields[0], "bdy") ==0){
                    float bdy[w_n->Nlat][w_n->Nlong];
                    for(int i = 0; i < w_n->Nlat; i++){
                        for(int j = 0; j < w_n->Nlong; j++ ){
                            get_line(ins);
                            bdy[i][j] = atof(ins->fields[0]);
                        }
                    }
                    if ((w_n->retval = nc_put_var_float(w_n->ncid, w_n->bdy_varid, &bdy[0][0])))
                        ERR(w_n->retval);
                }
            }
        }
    jettison_inputstruct(ins);

}
//read from a input file and write that to the netcdf (file should contain 24 hr data)
void read_file_write(char * fname, int var_offset, write_nc * w_n ){
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
                strcat(tmp,"_map_info.txt\0");
                
                //declare write_nc struct object, get the coordinats from the correct file
                //get the variable names
                w_n = (write_nc *) malloc(sizeof(write_nc));
                set_variables(TNtime, tmp, is, w_n);
                

                strcpy(tmp, main_name);
                strcat(tmp,".nc\0");
                //create the nc file and define all of the variables and exit define mode
                create_and_define(tmp, w_n);

                strcpy(tmp, main_name);
                strcat(tmp,"_map_info.txt\0");
                //write out the coordinate variable data
                write_region_info(tmp,w_n);
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
