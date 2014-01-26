# The name of the executable to produce
EXE = graph.exe
# The objects to link
OBJS = graph.o

# Compiler flags:
LIBS = -lmingw32 -lSDLmain -lSDL -lopengl32 -static-libgcc -static-libstdc++

# Put it all together and compile to .os:
$(EXE) : $(OBJS)
	g++ -o $(EXE) $(OBJS) $(LIBS)

# Compile all .cpps into .os:
%.o : %.cpp
	g++ $< -c
