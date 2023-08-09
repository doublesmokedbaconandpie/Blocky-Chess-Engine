# I'm pretty sure many IDEs handle compilation for us, but I'm creating a Makefile just in case.
# I don't have too much experience with Makefiles.

all: test

board:
	g++ src/board.cpp -o board.exe

test: 
	g++ tests/test_board.cpp -o test_board.exe

clean:
	rm -rf *.o
	rm -rf *.exe