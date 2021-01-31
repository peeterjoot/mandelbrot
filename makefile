CXXFLAGS += -std=c++17
CXXFLAGS += -MMD
OPTIMIZE := 1
ifdef OPTIMIZE
CXXFLAGS += -O2
else
CXXFLAGS += -g
endif
CXX := g++

#TARGETS += testit
TARGETS += mcomplex
TARGETS += mpauli
TARGETS += mga20

all : $(TARGETS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) $(filter %.cc,$^) -c -o $@

mcomplex : mandelbrot.o fcomplex.o
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@

mga20 : mandelbrot.o fga20.o
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@

mpauli : mandelbrot.o fpauli.o
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
