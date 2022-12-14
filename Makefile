# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/17 10:24:15 by tpeters           #+#    #+#              #
#    Updated: 2022/12/15 00:52:20 by tpeters          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = lexer.c parser.c evaluator.c tokens.c utils.c data.c executor.c \
		builtins.c env.c execve.c
MAIN = main.c
OBJS = $(SRCS:.c=.o)
MO = main.o
TESTS = test.c test_main.c
TOBJS = $(TESTS:.c=.o)
TEST = msh_test
NAME = minishell

DOWNLOADFOLDER=dwnlds

CFLAGS = -Wall -Wextra -I$(DOWNLOADFOLDER)/readline_out/include #-Werror
LINK_FLAGS = -Llibft -lft -L$(DOWNLOADFOLDER)/readline_out/lib -lreadline
CC = cc

LFT = libft
LFTLIB=$(LFT)/libft.a

ifeq ($(OS),Windows_NT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		LINK_FLAGS += -ltinfo
		LSANLFLAGS := -rdynamic -LLeakSanitizer -llsan -ldl -lstdc++
	endif
	ifeq ($(UNAME_S),Darwin)
		LSANLFLAGS := -LLeakSanitizer -llsan -lc++
	endif
endif

all: $(NAME)

$(DOWNLOADFOLDER):
	mkdir -p $(DOWNLOADFOLDER)
	curl -s https://ftp.gnu.org/gnu/readline/readline-8.1.2.tar.gz --output $(DOWNLOADFOLDER)/readline-8.1.2.tar.gz
	tar xfz $(DOWNLOADFOLDER)/readline-8.1.2.tar.gz -C $(DOWNLOADFOLDER)
	cd $(DOWNLOADFOLDER)/readline-8.1.2; ./configure --prefix=$(PWD)/dwnlds/readline_out; cd ../../
	$(MAKE) -C $(DOWNLOADFOLDER)/readline-8.1.2
	$(MAKE) install -C $(DOWNLOADFOLDER)/readline-8.1.2

$(LFT) :
	git clone git@github.com:Ludmuterol/libft.git

$(LFTLIB) : $(LFT)
	$(MAKE) -C libft bonus

LSAN = LeakSanitizer
LSANLIB = /LeakSanitizer/liblsan.a
lsan: CFLAGS += -ILeakSanitizer -Wno-gnu-include-next
lsan: LINK_FLAGS += $(LSANLFLAGS)
lsan: fclean $(LSANLIB)
lsan: all

$(LSAN):
	git clone https://github.com/mhahnFr/LeakSanitizer.git

$(LSANLIB): $(LSAN)
	$(MAKE) -C LeakSanitizer

debug: CFLAGS += -g
debug: all

$(NAME): $(SRCS) $(MAIN)  $(LFTLIB) $(DOWNLOADFOLDER)
	$(CC) $(CFLAGS) $(SRCS) $(MAIN) $(LINK_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS) $(MO)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME) cat echo

re: fclean
	$(MAKE)

test: $(TOBJS) $(OBJS)
	$(CC) $(TOBJS) $(OBJS) $(LINK_FLAGS) -o $(TEST)
	./$(TEST)

run: all
	./$(NAME)

tools: $(LFTLIB)
	gcc echo.c -o echo -Llibft -lft
	gcc cat.c -o cat -Llibft -lft


.PHONY: lsan debug all clean fclean re test
