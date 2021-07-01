INCFLAGS  = -I /usr/include/GL
<<<<<<< HEAD
INCFLAGS += -I /mit/6.837/public/include/vecmath

LINKFLAGS  = -lglut -lGL -lGLU
LINKFLAGS += -L /mit/6.837/public/lib -lvecmath
=======
INCFLAGS += -I ./include/vecmath

LINKFLAGS  = -lglut -lGL -lGLU
#LINKFLAGS += -L /mit/6.837/public/lib -lvecmath
>>>>>>> f2fa124e8b132665cdaacbe512c17a59111bd0be

CFLAGS    = -O2
CC        = g++
SRCS      = main.cpp
<<<<<<< HEAD
=======
SRCS     += $(wildcard vecmath/*.cpp)
>>>>>>> f2fa124e8b132665cdaacbe512c17a59111bd0be
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

<<<<<<< HEAD
=======
# DO NOT DELETE

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

>>>>>>> f2fa124e8b132665cdaacbe512c17a59111bd0be
