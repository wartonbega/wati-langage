
build:
	echo "Build pour Macos..."
	g++ -O3 -std=c++17 -pthread ./src/*.cpp -o ./bin/wati 

run:
	g++ -O3 -std=c++17 ./src/*.cpp -o ./bin/wati 
	./bin/wati test.wati

debug:
	g++ -g -O3 -std=c++17 ./src/*.cpp -o a.out

cpp_lib_macos:
	g++ -dynamiclib -flat_namespace cpp_lib/*.cpp src/*.cpp -o cpp_lib/libs.so -std=c++17 -I include -L lib -l SDL2-2.0.0 -lSDL2_image -lSDL2_ttf -O3 -ldl
	cp -R ./cpp_lib /usr/local/lib/wati/

cpp_lib_linux:
	g++ -shared cpp_lib/*.cpp src/*.cpp -o cpp_lib/libs.so -std=c++17 -I include -L lib -l SDL2-2.0.0 -lSDL2_image -lSDL2_ttf -O3 -ldl
	cp -R ./cpp_lib /usr/local/lib/wati/