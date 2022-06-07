CC = gcc 
INCLUDES = -I. -I./libfdr/
EXECUTABLES = get_required_info make_nc
CFLAGS = $(INCLUDES)
Lib = -lnetcdf ./libfdr/libfdr.a
all: $(EXECUTABLES)

get_required_info: get_required_info.c
	$(CC) -ggdb3 $(CFLAGS) -o get_required_info get_required_info.c

make_nc: make_nc.c
	$(CC) -ggdb3 $(CFLAGS) -o make_nc make_nc.c $(Lib)

clean:
	rm core $(EXECUTABLES) *.o
