# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -g -fpermissive -std=c++11

# define the executable file 
TARGET = Cities

all: $(TARGET)

Cities: Cities.C
	$(CC) $(CFLAGS) -o Cities Cities.C

clean:
	$(RM) *.o *~ $(TARGET)

