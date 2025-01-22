PROG = game
CC = g++
# CPPFLAGS = -Wall -Wextra -Wpedantic -O2
CPPFLAGS = 
LIBFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lv8
V8FLAGS = -I/usr/include -I/usr/include/v8 -L/usr/lib/aarch64-linux-gnu
VERFLAGS = -std=c++17
THRDFLAGS = -lpthread
NTWRKFLAGS = -lzmq
OBJS = NetworkingClient.o EventHandler.o ReplayHandler.o ThreadMaker.o Timeline.o GameObject.o HiddenObjects.o Platform.o Player.o v8helpers.o ScriptManager.o Game.o main.o

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBFLAGS) $(VERFLAGS) $(THRDFLAGS) $(NTWRKFLAGS)

NetworkingClient.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/NetworkingClient.cpp $(V8FLAGS)

EventHandler.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/EventHandler.cpp $(V8FLAGS)

ReplayHandler.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/ReplayHandler.cpp $(V8FLAGS)

ThreadMaker.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/ThreadMaker.cpp $(V8FLAGS)

Timeline.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/Timeline.cpp $(V8FLAGS)

GameObject.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/GameObject.cpp $(V8FLAGS)

HiddenObjects.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/HiddenObjects.cpp $(V8FLAGS)

Platform.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/Platform.cpp $(V8FLAGS)

Player.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/Player.cpp $(V8FLAGS)

v8helpers.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/v8helpers.cpp $(V8FLAGS)

ScriptManager.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/ScriptManager.cpp $(V8FLAGS)

Game.o:
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/Game.cpp $(V8FLAGS)

main.o: 
	$(CC) $(CPPFLAGS) $(VERFLAGS) -c ../src/main.cpp $(V8FLAGS)


.PHONY: clean
clean:
	rm -f $(obj) main.o
	rm -f Game.o
	rm -f ScriptManager.o
	rm -f v8helpers.o
	rm -f Timeline.o
	rm -f ThreadMaker.o
	rm -f NetworkingClient.o
	rm -f GameObject.o
	rm -f HiddenObjects.o
	rm -f Player.o
	rm -f Platform.o
	rm -f EventHandler.o
	rm -f ReplayHandler.o
	rm -f game
	clear

build:
	rm -f $(obj) main.o
	rm -f Game.o
	rm -f ScriptManager.o
	rm -f v8helpers.o
	rm -f Timeline.o
	rm -f ThreadMaker.o
	rm -f NetworkingClient.o
	rm -f GameObject.o
	rm -f HiddenObjects.o
	rm -f Player.o
	rm -f Platform.o
	rm -f EventHandler.o
	rm -f ReplayHandler.o
	clear