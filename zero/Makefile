INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./include/vecmath

LINKFLAGS  = -lglut -lGL -lGLU
#LINKFLAGS += -L /mit/6.837/public/lib -lvecmath

APP 	 = simplify
HPPS 	 = mesh.hpp
OBJECTS = main.o

CFLAGS    = -O2 -g
CC        = g++
SRCS      = main.cpp
SRCS     += $(wildcard vecmath/*.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

# default:
# 	make $(APP)

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)
mesh.o: $(SRCS)
	$(CC) $(CFLAGS) mesh.hpp -o $@ $(INCFLAGS) $(LINKFLAGS)



.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

$(APP): $(OBJECTS) Makefile
	echo 'Linking: $(APP)' && \
	$(CC) $(INCFLAGS) $(LINKFLAGS) $(OBJECTS) -o $(APP)

$(ENTRANCE).o: $(ENTRANCE).cpp $(HPPS) Makefile
	echo 'Compiling: $(ENTRANCE).o:'	&& \
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o $(INCFLAGS) $(LINKFLAGS)

%.o: %.cpp %.h Makefile
	echo 'Compiling: $*.o' && \
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o $(INCFLAGS) $(LINKFLAGS)


depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

# DO NOT DELETE
sim.o: ./sim.h

main.o: ./include/vecmath/vecmath.h ./include/vecmath/Matrix2f.h
main.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Matrix4f.h
main.o: ./include/vecmath/Quat4f.h ./include/vecmath/Vector2f.h
main.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Vector3f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector2f.h
vecmath/Vector2f.o: ./include/vecmath/Vector2f.h ./include/vecmath/Vector3f.h
vecmath/Matrix3f.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Matrix2f.h
vecmath/Matrix3f.o: ./include/vecmath/Quat4f.h ./include/vecmath/Vector3f.h
vecmath/Matrix4f.o: ./include/vecmath/Matrix4f.h ./include/vecmath/Matrix2f.h
vecmath/Matrix4f.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Quat4f.h
vecmath/Matrix4f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Vector4f.o: ./include/vecmath/Vector4f.h ./include/vecmath/Vector2f.h
vecmath/Vector4f.o: ./include/vecmath/Vector3f.h
vecmath/Quat4f.o: ./include/vecmath/Quat4f.h ./include/vecmath/Matrix3f.h
vecmath/Quat4f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Matrix2f.o: ./include/vecmath/Matrix2f.h ./include/vecmath/Vector2f.h

