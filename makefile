build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp buildings.cpp footway.cpp footways.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp dist.cpp graph.cpp -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	valgrind --tool=memcheck --leak-check=full ./a.out

clean:
	rm -f ./a.out