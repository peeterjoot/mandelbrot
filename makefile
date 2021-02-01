CXXFLAGS += -std=c++17
CXXFLAGS += -MMD
OPTIMIZE := 1
ifdef OPTIMIZE
CXXFLAGS += -O2
else
CXXFLAGS += -g
endif
CXX := g++

ifdef WSL_DISTRO_NAME
CXXFLAGS += $(shell /usr/bin/GraphicsMagick++-config --cppflags)
LOADLIBES += $(shell /usr/bin/GraphicsMagick++-config --libs)
else
# fedora:
CXXFLAGS += -I/usr/include/ImageMagick-6
LOADLIBES += -lMagick++-6.Q16
endif

#TARGETS += testit
TARGETS += mcomplex
TARGETS += mpauli
TARGETS += mga20

define LINKRULE
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LOADLIBES)
endef

all : $(TARGETS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) $(filter %.cc,$^) -c -o $@

mcomplex : mandelbrot.o fcomplex.o
	$(LINKRULE)

mga20 : mandelbrot.o fga20.o
	$(LINKRULE)

mpauli : mandelbrot.o fpauli.o
	$(LINKRULE)

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
