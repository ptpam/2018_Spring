CC=g++ -std=c++11
CFLAGS=-I.
DEPS = TeapotControlPoints.h Shader.h GeometricDefinitions.h Object3D.h
OBJ = deneme.o 
LIBS = -lglut -lGL -lGLEW -lglfw
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

deneme: $(OBJ)
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS) && ./deneme