OB += segmentation.o
	

%.o:
	g++  -O0 -g3 -Wall -c  segmentation.cpp `pkg-config --cflags --libs opencv`

all:$(OB)
	@echo 'Building target: $@'
	g++  -o "segmentation" $(OB)  `pkg-config --cflags --libs opencv`
	@echo 'Finished building target: $@'

clear:
	rm -rf *.o
