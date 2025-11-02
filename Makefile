all: rangejoin test

rangejoin: rangejoin.cpp rangejoin.hpp
	c++ --std=c++20 -I$(shell brew --prefix boost)/include rangejoin.cpp -o rangejoin

test: rangejoin
	./rangejoin

clean:
	rm -f rangejoin
