TARGET := compressioneDCT
SRC_DIR = CompressioneDCT
CC_FLAGS := `pkg-config --cflags --libs gtk+-3.0`

all: main
	g++ main.o -o $(SRC_DIR)/compressioneDCT $(CC_FLAGS) -lfftw3
main: 
	g++ -c $(SRC_DIR)/main.cpp -o main.o $(CC_FLAGS)

install-dep:
	apt-get update
	apt-get install -y g++ libgtk-3-dev libfftw3-3 libfftw3-dev

clean:
	@rm -rf *.o
