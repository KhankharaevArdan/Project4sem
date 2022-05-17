CC=g++
CFLAGS=-c -Wall -std=c++17 -IIncludes

all: game

game: clean main.o Game.o Objects.o NetWorkServer.o NetWorkClient.o Heroes.o
	$(CC) main.o Game.o Objects.o NetWorkClient.o NetWorkServer.o Heroes.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

main.o:
	$(CC) $(CFLAGS) main.cpp

Game.o:
	$(CC) $(CFLAGS) ./Source/Game.cpp

Objects.o:
	$(CC) $(CFLAGS) ./Source/Objects.cpp

NetWorkServer.o :
	$(CC) $(CFLAGS) ./Source/NetWorkServer.cpp

NetWorkClient.o :
	$(CC) $(CFLAGS) ./Source/NetWorkClient.cpp

Heroes.o :
	$(CC) $(CFLAGS) ./Source/Heroes.cpp

clean:
	rm -rf *.o game