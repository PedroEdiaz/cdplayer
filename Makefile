BIN= cdplayer
OBJ= \
	cdplayer.o \
	tinyalsa/src/pcm.o \
	tinyalsa/src/pcm_hw.o \
	tinyalsa/src/snd_card_plugin.o

CFLAGS = -Itinyalsa/include

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)
