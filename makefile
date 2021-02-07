CXXFLAGS += -std=c++17
CXXFLAGS += -MMD
#OPTIMIZE := 1
ifdef OPTIMIZE
CXXFLAGS += -O2
else
CXXFLAGS += -g
endif
CXX := g++

CXXFLAGS += -DHAVE_IMAGEMAGICK
HAVE_IMAGEMAGICK := 1

UNAME := $(shell uname)
ifeq (Darwin,$(UNAME))
	MACOSX := 1
endif

# brew install imagemagick
ifdef MACOSX
	MAGICKCONFIG := /usr/local/bin/Magick++-config
endif
# WSL+ubuntu:
ifdef WSL_DISTRO_NAME
	MAGICKCONFIG := /usr/bin/GraphicsMagick++-config
endif

ifdef MAGICKCONFIG
	CXXFLAGS += $(shell $(MAGICKCONFIG) --cppflags)
	LOADLIBES += $(shell $(MAGICKCONFIG) --libs)
else
	# fedora:
	CXXFLAGS += -I/usr/include/ImageMagick-6
	LOADLIBES += -lMagick++-6.Q16
endif

#CXXFLAGS += -DNETCDF_SUPPORTED
ifdef MACOSX
LOADLIBES += -lnetcdf-cxx4
else
LOADLIBES += -lnetcdf_c++4
endif

#TARGETS += testit
TARGETS += mcomplex
TARGETS += mpauli
TARGETS += mga20

define LINKRULE
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LOADLIBES) $(LOADLIBES_$@)
endef

all : $(TARGETS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) $(filter %.cc,$^) -c -o $@

mcomplex : mandelbrot.o fcomplex.o
	$(LINKRULE)

writefile : writefile.o
	$(LINKRULE)

mga20 : mandelbrot.o fga20.o
	$(LINKRULE)

mpauli : mandelbrot.o fpauli.o
	$(LINKRULE)

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
