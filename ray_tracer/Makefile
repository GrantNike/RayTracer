LIBS = -Iglm -lfreeimage
CFLAGS = -std=c++11
INC=glm lfreeimage
INC_PARAMS=$(foreach d, $(INC), -I$(INC_DIR)$d)

load: ray_tracer.cpp
	g++ $(CFLAGS) -o ray_tracer ray_tracer.cpp $(LIBS)