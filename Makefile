###########################################################
#### NAME

NAME		= philo

###########################################################
#### PATH TO SOURCES

PATH_SRCS	+= srcs/

###########################################################
#### SOURCES

SRCS		+= main.c
SRCS		+= init.c
SRCS		+= time.c
SRCS		+= ft_atoi.c
SRCS		+= free.c

vpath %.c $(PATH_SRCS)

###########################################################
#### VALGRIND

VALGRIND	+= valgrind
VALGRIND	+= --track-origins=yes
VALGRIND	+= --leak-check=full
VALGRIND	+= --show-leak-kinds=all

###########################################################
#### OBJS

PATH_OBJS	= objs/
OBJS		= $(patsubst %.c, $(PATH_OBJS)/%.o, $(SRCS))

###########################################################
#### INCLUDES

INCLUDES	= -I./includes
HEADER		= includes/philo.h

###########################################################
#### COMPILATION

CC				= cc
CFLAGS			+= -Wall
CFLAGS			+= -Wextra
CFLAGS			+= -Werror
CFLAGS			+= -fsanitize=address
# CFLAGS			+= -fsanitize=thread

SMAKE		= make --no-print-directory

###########################################################
#### RULES

all : $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -L $(INCLUDES) -pthread
	@echo "\033[1;35m======= philo has been created =======\033[0m"

$(OBJS): $(PATH_OBJS)/%.o: %.c $(HEADER)
	@mkdir -p $(PATH_OBJS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@$(RM) -R $(PATH_OBJS)
	@echo "\033[1;36m======= philo object files have been removed =======\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[1;36m======= philo has been removed =======\033[0m"

re: fclean
	@$(SMAKE)

norm:
	@norminette $(PATH_SRCS) $(INCLUDES)

valgrind:
	@$(VALGRIND) ./$(NAME)

.PHONY: all clean fclean re norm valgrind

# variables spéciales
# $@ fait référence au nom de la cible
# $< nom première dépendance
# $^ liste dépendance
# $? liste dépendance plus récentes que la cible
# $* nom fichier, sans son extension
# CIBLE > DEPENDANCES > REGLES
