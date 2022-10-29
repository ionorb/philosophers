NAME			=	philo

SOURCES_DIR		=	sources

SOURCES_FILES	=	philo.c utils.c

SOURCES			=	$(addprefix $(SOURCES_DIR)/, $(SOURCES_FILES))

HEADER			=	$(SOURCES_DIR)/philo.h

OBJECTS			=	$(SOURCES:.c=.o)

CC 				=	gcc

RM				=	rm -f

CFLAGS			=	-g3 -Wall -Wextra -Werror

PFLAGS			=	-lpthread

.c.o:		
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJECTS) $(HEADER)
	$(CC) $(CFLAGS) $(PFLAGS) $(OBJECTS) -o $(NAME)
	$(RM) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:		all clean fclean re
