
build:
	g++ -O3 -std=c++17 -pthread ./src/*.cpp -o ./bin/wati1.2 -ldl

build_install:
	g++ -O3 -std=c++17 -pthread ./src/*.cpp -o ./bin/wati1.2 -ldl
	sudo cp ./bin/wati1.2 /usr/local/bin/wati1.2


build_linux:
	g++ -O3 -std=c++17 -pthread ./src/*.cpp -o ./bin/wati1.2 -ldl

run:
	g++ -O3 -std=c++17 ./src/*.cpp -o ./bin/wati1.2 
	./bin/wati1.2 test.wati

debug:
	g++ -O3 -std=c++17 -pthread ./src/*.cpp -o ./bin/wati1.2 -ldl -g

cpp_lib_macos:
	g++ -dynamiclib -flat_namespace cpp_lib/*.cpp src/*.cpp -o cpp_lib/libs.so -std=c++17 -I include -L lib  -O3 -ldl
	sudo cp -R ./cpp_lib /usr/local/lib/wati1.2/

cpp_lib_macos_debug:
	g++ -dynamiclib -flat_namespace cpp_lib/*.cpp src/*.cpp -o cpp_lib/libs.so -std=c++17 -I include -L lib  -O3 -ldl -g
	sudo cp -R ./cpp_lib /usr/local/lib/wati1.2/

cpp_lib_linux:
	g++ -shared cpp_lib/*.cpp src/*.cpp -o cpp_lib/libs.so -std=c++17  -O3 -fPIC
	sudo cp -R ./cpp_lib /usr/local/lib/wati1.2/