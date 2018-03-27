CXX=g++
OPTIM = -O3 -march=native
DEBUG = -g -Wall

OPTC  = ${OPTIM} -std=c++11

SRCS = src

all: animation_transform.exe

animation_transform.exe: src/animation_transform.cpp src/contrast.hpp  src/convolution.hpp  src/detect_bord.hpp  src/flou.hpp  src/gauss3.hpp  src/image.hpp  src/renforce_bord.hpp  src/repoussage.hpp
	$(CXX) $(OPTC) -Isrc/ src/animation_transform.cpp src/lodepng.cpp -o animation_transform.exe

clean:
	rm -f *.exe src/*~ data/*/*_out.*