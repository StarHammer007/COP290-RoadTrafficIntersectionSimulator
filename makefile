all: rend v road comp removeoutput
v:
	g++ -std=c++11 Vehicle.cpp -c
rend:
	g++ -std=c++11 Render.cpp -c -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
road:
	g++ -std=c++11 Road.cpp -c
comp:
	g++ -std=c++11 -o main main.cpp Road.o Vehicle.o Render.o -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
removeoutput:
	rm -rf output.txt
clean:
	rm -rf *.o main

test:
	g++ -std=c++11 test.cpp -o test -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
