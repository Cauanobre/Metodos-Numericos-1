all:
	g++ -o main main.cpp -Wall -Wextra -Wpedantic -std=c++20
 
run:
	./main
 
clean:
	rm -f main *.csv