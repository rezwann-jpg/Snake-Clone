all:
	g++ main.cpp Snake.cpp Food.cpp -o bin/main -I include/ -L lib/ -lSDL2main -lSDL2 -lSDL2_ttf -lmingw32
run:
	./bin/main