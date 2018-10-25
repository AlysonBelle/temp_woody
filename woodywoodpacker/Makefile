



EXEC	= 	woody_woodpacker

SRC	= 	main.c \
		update_offsets.c \
		write_file.c \
		convert_address.c \
		woodywoodpacker.c \
		mapping.c

OBJ	=	$(SRC:.c=.o)

LIB 	= 	alylibc/lib.a

all:
	make -C alylibc
	gcc -Wall -Werror -Wextra -c $(SRC)
	gcc -o $(EXEC) $(OBJ) $(LIB)

clean:
	make clean -C alylibc
	rm -f $(OBJ)

fclean:
	make fclean -C alylibc
	rm -f $(OBJ)
	rm -f $(EXEC)

re: fclean all
