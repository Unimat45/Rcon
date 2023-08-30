CC		:= clang
C_FLAGS	:= -O2 -Wall -Wextra -std=c11

SRC		:= src
BIN		:= bin
INCLUDE := include

LIB		:= 
EXEC	:= rcon

all: $(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(SRC)/*.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIB)

clean:
	rm bin/*