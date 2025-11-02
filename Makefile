all: rangejoin test

rangejoin: test/test.cpp src/rangejoin.hpp
	c++ --std=c++20 -Isrc test/test.cpp -o test/test

test: rangejoin
	./test/test

clean:
	rm -f test/test
