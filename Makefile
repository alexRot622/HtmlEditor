CC = gcc
CFLAG = -Wall -Wextra -g
EXE = tema3
OBJ = utils.o list.o stack.o tree.o attr.o interpret.o selector.o cmds.o main.o

build: $(EXE)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): $(OBJ)
	$(CC) -g $^ $(CFLAG) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -f *.o $(EXE)
