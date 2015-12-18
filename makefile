CXX = g++
CFLAGS = -I.
LIBS = -lm -lbcm2835
DEPS = Ks0108pi.h
OBJ = main.o Ks0108pi.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
