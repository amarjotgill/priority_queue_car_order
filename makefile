CXX = g++
CXXFLAGS = -Wall

test: mqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) mqueue.o mytest.cpp -o test

driver: mqueue.o driver.cpp
	$(CXX) $(CXXFLAGS) mqueue.o driver.cpp -o driver

mqueue.o: mqueue.h mqueue.cpp
	$(CXX) $(CXXFLAGS) -c mqueue.cpp

run:
	./test

run2:
	./driver
	
val:
	valgrind ./test

val2:
	valgrind ./driver

clean:
	rm *~
	rm *.o