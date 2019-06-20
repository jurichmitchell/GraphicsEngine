##
## Mitchell Jurich
## Last Edit: 11/30/2018
## Windows makefile 
##

CC = g++
CFLAGS = -Wall -O3 -g -w
INCLUDES = -I"C:\glew-2.0.0\include" -I"C:\freeglut\include" -I"C:\glfw-3.2\include"
LIBS = -L"C:\glew-2.0.0\lib\Release\Win32" -L"C:\freeglut\lib" -L"C:\glfw-3.2\lib-mingw" -lglew32s -lglfw3 -lfreeglut -lopengl32 -lgdi32 -lglu32

RENDER: FileReader.o Window.o View.o CubeMap.o InitShader.o Main.cpp
	$(CC) $(CFLAGS) -o render Main.cpp FileReader.o Window.o View.o CubeMap.o Object.o TransformMaker.o Face.o Group.o Material.o Model.o Light.o Color.o InitShader.o $(INCLUDES) $(LIBS)
	make clean

InitShader.o: InitShader.cpp
	$(CC) $(CFLAGS) InitShader.cpp $(INCLUDES) -c

Window.o: Window.cpp Window.h
	$(CC) $(CFLAGS) Window.cpp $(INCLUDES) -c

View.o: View.cpp View.h
	$(CC) $(CFLAGS) View.cpp $(INCLUDES) -c

CubeMap.o: stb_image.h CubeMap.cpp CubeMap.h
	$(CC) $(CFLAGS) CubeMap.cpp $(INCLUDES) -c
	
FileReader.o: FileReader.cpp FileReader.h Object.o Face.o Material.o Model.o Color.o Group.o Light.o
	$(CC) $(CFLAGS) FileReader.cpp $(INCLUDES) -c

Model.o: Model.cpp Model.h Object.o
	$(CC) $(CFLAGS) Model.cpp $(INCLUDES) -c

Object.o: Object.cpp Object.h Angel.h Group.o TransformMaker.o
	$(CC) $(CFLAGS) Object.cpp $(INCLUDES) -c
	
TransformMaker.o: TransformMaker.cpp TransformMaker.h
	$(CC) $(CFLAGS) TransformMaker.cpp $(INCLUDES) -c
	
Group.o: Group.cpp Group.h Material.o
	$(CC) $(CFLAGS) Group.cpp $(INCLUDES) -c
	
Material.o: Material.cpp Material.h Color.o
	$(CC) $(CFLAGS) Material.cpp $(INCLUDES) -c

Light.o: Light.cpp Light.h Color.o
	$(CC) $(CFLAGS) Light.cpp $(INCLUDES) -c

Color.o: Color.cpp Color.h
	$(CC) $(CFLAGS) Color.cpp $(INCLUDES) -c
	
Face.o: Face.cpp Face.h
	$(CC) $(CFLAGS) Face.cpp $(INCLUDES) -c
	
all:
	RENDER
	
clean:
	del *.o
