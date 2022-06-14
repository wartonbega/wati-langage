
build:
	g++ -O3 -std=c++17 -lcurses ./src/*.cpp -o ./bin/wati 

run:
	g++ -O3 -std=c++17 -lcurses ./src/*.cpp -o ./bin/wati 
	./bin/wati test.wati

debug:
	g++ -g -O3 -std=c++17 ./src/*.cpp -o a.out