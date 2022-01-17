CC = gcc 
#please replace the file paths to where the includes for netcdf are located
INCLUDES = -I/home/npatel79/builds/work/libs/nc4libs/include -I.
EXECUTABLES =  convert
CFLAGS = $(INCLUDES)
Lib = -L/home/npatel79/builds/work/libs/nc4libs/lib/ -lnetcdf
all: $(EXECUTABLES)
convert: convert.c
	$(CC) $(CFLAGS) -o convert convert.c

clean:
	rm core $(EXECUTABLES) *.o
# export LD_LIBRARY_PATH=/home/npatel79/builds/work/libs/nc4libs/lib:$LD_LIBRARY_PATH

