CC = gcc 
INCLUDES = -I. -I./libfdr/
EXECUTABLES = get_input_info make_nc
CFLAGS = $(INCLUDES)
Lib = -lnetcdf ./libfdr/libfdr.a
all: $(EXECUTABLES)

get_input_info: get_input_info.c
	$(CC) -ggdb3 $(CFLAGS) -o get_input_info get_input_info.c

make_nc: make_nc.c
	$(CC) -ggdb3 $(CFLAGS) -o make_nc make_nc.c $(Lib)

clean:
	rm $(EXECUTABLES)
