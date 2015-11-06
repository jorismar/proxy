CC = g++

all:
	$(CC) src/*.cpp -lgpac -I include/ -o main

clean:
	rm main