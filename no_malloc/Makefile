NAME			=	philo

SOURCES_DIR		=	sources

SOURCES_FILES	=	main.c actions.c utils.c

SOURCES			=	$(addprefix $(SOURCES_DIR)/, $(SOURCES_FILES))

HEADER			=	philo.h

OBJECTS			=	$(SOURCES:.c=.o)

CC 				=	cc

RM				=	rm -f

CFLAGS			=	-g3 -Wall -Wextra -Werror -pthread

SFLAGS			=	-g3 -fsanitize=thread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJECTS) $(HEADER)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

sanitize:	$(OBJECTS) $(HEADER)
	$(CC) $(SFLAGS) $(CFLAGS) $(OBJECTS) -o $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:		all clean fclean re
