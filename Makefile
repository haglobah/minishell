# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/17 10:24:15 by tpeters           #+#    #+#              #
#    Updated: 2022/09/17 10:24:15 by tpeters          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = 
OBJS = $(SRCS:.c=.o)
NAME = 

CFLAGS = -Wall -Wextra -Werror
LINK_FLAGS = -Llibft -lft
CC = cc

ifeq ($(OS),Windows_NT)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
    endif
    ifeq ($(UNAME_S),Darwin)
    endif
endif

LIBFTLIB=/libft/libft.a
all: $(LIBFTLIB)
all: $(NAME) 

$(LIBFTLIB):
	if [ ! -d "libft" ]; then git clone git@github.com:Ludmuterol/libft.git; fi
	$(MAKE) -C libft

LSANLIB = /LeakSanitizer/liblsan.a
lsan: CFLAGS += -ILeakSanitizer -Wno-gnu-include-next
lsan: LINK_FLAGS += -LLeakSanitizer -llsan -lc++
lsan: fclean $(LSANLIB)
lsan: all

$(LSANLIB): 
	if [ ! -d "LeakSanitizer" ]; then git clone git@github.com:mhahnFr/LeakSanitizer.git; fi
	$(MAKE) -C LeakSanitizer

debug: CFLAGS += -g
debug: all

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)

.PHONY: lsan debug all clean fclean re