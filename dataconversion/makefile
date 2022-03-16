CC = gcc 
#please replace the file paths to where the includes for netcdf are located
INCLUDES = -I/home/npatel79/builds/work/libs/nc4libs/include -I. -I./libfdr/
EXECUTABLES = get_required_info make_nc
CFLAGS = $(INCLUDES)
Lib = -L/home/npatel79/builds/work/libs/nc4libs/lib/ -lnetcdf ./libfdr/libfdr.a
all: $(EXECUTABLES)
get_required_info: get_required_info.c
	$(CC) -ggdb3 $(CFLAGS) -o get_required_info get_required_info.c

make_nc: make_nc.c
	$(CC) -ggdb3 $(CFLAGS) -o make_nc make_nc.c $(Lib)

clean:
	rm core $(EXECUTABLES) *.o
# export LD_LIBRARY_PATH=/home/npatel79/builds/work/libs/nc4libs/lib:$LD_LIBRARY_PATH

#read me tell them to self install; 