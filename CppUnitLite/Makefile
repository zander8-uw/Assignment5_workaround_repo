CC=g++
CFLAGS=-I.

OBJ = Failure.o Test.o TestRegistry.o TestResult.o WFailure.o
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

libCppUnitLite.a: $(OBJ)
	ar rcs libCppUnitLite.a $(OBJ)
	ranlib libCppUnitLite.a

all: libCppUnitLite.a

clean:
	rm *.a
	rm *.o
