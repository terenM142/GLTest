CC = g++
SOURCES = file_find.cpp driver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = findfile

library: file_find.cpp file_find.h file_find.o
	$(CC) -c file_find.cpp -o file_find.o
	ar rvs file_find.a file_find.o

drive: file_find.a driver.cpp
	$(CC) driver.cpp file_find.a -o $(TARGET)

clean:
	rm -f $(OBJECTS) file_find.a $(TARGET)


