CPPFLAGS = -std=c++20 -Wall -Wextra -Wshadow -pedantic -march=native
HEADERS = include/bitarray.hpp

.PHONY: clean debug

main: main.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DNDEBUG -Ofast -o main main.cpp

clean:
	rm -f main

debug: main.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DDEBUG -g -o main main.cpp

%.hpp:
