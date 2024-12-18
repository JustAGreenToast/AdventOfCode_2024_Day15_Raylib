NAME=AoC_2024_Day15

FILES=	main.cpp \
		Vec2Int.cpp \
		Grid.cpp
SRC=$(addprefix src/, $(FILES))
OBJ=$(addprefix obj/, $(FILES:.cpp=.o))
DEP=$(addprefix obj/, $(FILES:.cpp=.d))

RAYLIB=raylib/src/libraylib.a
RAYLIB_MAKE=make -C raylib/src

CC=c++
CFLAGS=-Wall -Wextra -Werror -std=c++11

RM=rm -rf

all: $(NAME)

$(NAME): raylib $(OBJ) Makefile
	$(CC) -o $(NAME) $(OBJ) $(RAYLIB)

obj/%.o: src/%.cpp Makefile
	mkdir -p obj/
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

raylib:
	$(RAYLIB_MAKE)

clean:
	$(RAYLIB_MAKE) clean
	$(RM) obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEP)
.PHONY: all raylib clean fclean re