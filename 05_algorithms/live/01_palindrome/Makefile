PROGRAM = 01_palindrome
CXX       = g++
CXXFLAGS  = -g -std=c++17 -Wall

$(PROGRAM): main.o
	$(CXX) -o $(PROGRAM) main.o

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


