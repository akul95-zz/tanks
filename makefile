CC = gcc
DEPS = game_io.h game_network.h game.h
OBJ =  game_io.o game_network.o game.o driver.o
CFLAGS = -pthread

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

game_exe: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *.o
	rm game_exe