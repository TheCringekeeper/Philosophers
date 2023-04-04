###########################################################
#### NAME

NAME		= philo

###########################################################
#### PATH TO SOURCES

PATH_SRCS	+= srcs/

###########################################################
#### SOURCES

SRCS		+= free.c
SRCS		+= ft_atoi.c
SRCS		+= ft_philo.c
SRCS		+= init.c
SRCS		+= main.c
SRCS		+= monitoring.c
SRCS		+= time.c
SRCS		+= utils.c

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
#### SPECIAL MODE ####
MODE			= 0
ifeq ($(MODE), thread)
	CFLAGS += -g -fsanitize=thread
endif
ifeq ($(MODE), address)
	CFLAGS += -g -fsanitize=address
endif

SMAKE		= make --no-print-directory

###########################################################
#### RULES

all : $(NAME)

$(NAME): $(OBJS)
	@clear
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -L $(INCLUDES) -pthread
	@echo "\033[1;35m           __    _ __                       __                  "
	@echo "    ____  / /_  (_) /___  _________  ____  / /_  ___  __________"
	@echo "   / __ \/ __ \/ / / __ \/ ___/ __ \/ __ \/ __ \/ _ \/ ___/ ___/"
	@echo "  / /_/ / / / / / / /_/ (__  ) /_/ / /_/ / / / /  __/ /  (__  ) "
	@echo " / .___/_/ /_/_/_/\____/____/\____/ .___/_/ /_/\___/_/  /____/  "
	@echo "/_/                              /_/                            \n\033[0m"

$(OBJS): $(PATH_OBJS)/%.o: %.c $(HEADER)
	@mkdir -p $(PATH_OBJS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@clear
	@$(RM) -R $(PATH_OBJS)
	@echo "\033[1;36m======== philo object files removed ========\033[0m"

fclean: clean
	@clear
	@$(RM) $(NAME)
	@echo "\033[1;36m======= philo executable removed =======\033[0m"

re: fclean
	@$(SMAKE)

.PHONY: all clean fclean re

# variables spéciales
# $@ fait référence au nom de la cible
# $< nom première dépendance
# $^ liste dépendance
# $? liste dépendance plus récentes que la cible
# $* nom fichier, sans son extension
# CIBLE > DEPENDANCES > REGLES
