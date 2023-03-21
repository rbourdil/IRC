ERASELINE=\r\033[K
UPFIVE=\033[5A
UP=\033[1A
DOWNFOUR=\033[4B

BLUE=\033[34m
RED=\033[31m
NOCOLOR=\033[0m

NAME	= ircserv
CC		= c++
FLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS	= $(wildcard ./sources/*.cpp)
INCS    = $(wildcard ./includes/*.hpp) $(wildcard ./includes/*/*.hpp)

INCPATH = -I./includes -I.
OBJ_DIR	= obj

OBJS	= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

all: $(rambo) $(OBJ_DIR) $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: ./sources/%.cpp
	$(CC) $(FLAGS) -MMD -o $@ -c $< $(INCPATH)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: 
	$(MAKE) FLAGS+='-DDEBUG=1' all

-include $(OBJ_DIR)/*.d

rambo:
	@echo '                      /^--^\     /^--^\     /^--^\'
	@echo '                      \____/     \____/     \____/'
	@echo '                     /      \   /      \   /      \'
	@echo '                    |        | |        | |        |'
	@echo '                     \__  __/   \__  __/   \__  __/'
	@echo '|^|^|^|^|^|^|^|^|^|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|^|^|^|^|^|^|'
	@echo '| | | | | | | | | | | | |\ \| | |/ /| | | | | | \ \ | | | | | | | | | | |'
	@echo '| | | | | | | | | | | | / / | | |\ \| | | | | |/ /| | | | | | | | | | | |'
	@echo '| | | | | | | | | | | | \/| | | | \/| | | | | |\/ | | | | | | | | | | | |'
	@echo '                                                                         '
	@echo '                .........   .......   ..........                         '
	@echo '                    .       .     .   .                                  '
	@echo '                    .       .......   .                                  '
	@echo '                    .       . .       .                                  '
	@echo '                    .       .   .     .                                  '
	@echo '                .........   .     .   ..........                         '
	@echo '                                                                         '
	@echo '| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |'
	@echo '| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |'

.PHONY: all clean fclean re rambo debug
