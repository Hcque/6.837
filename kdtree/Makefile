
output: test.o kdtree.o
	g++ test.o kdtree.o -o output

test.o: test.cpp
	g++ -c test.cpp

kdtree.o: kdtree.cpp kdtree.hpp
	g++ -c kdtree.cpp

clean:
	rm *.o output