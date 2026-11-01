all:
	g++ -o main src/main.cpp -Isrc -Wall -Wextra -Wpedantic -std=c++20
 
run:
	./main
 
clean:
	rm -f main *.csv
