CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
LDFLAGS = -lglfw -lGL -lGLU -lm -lnoise
SRC = terrain.c
EXECUTABLE = terrain

# Directory where the LibNoise headers are located
LIBNOISE_INCLUDE_DIR = /usr/include/noise

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(LIBNOISE_INCLUDE_DIR) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
