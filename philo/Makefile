# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaudot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/09 14:48:17 by abaudot           #+#    #+#              #
#    Updated: 2022/02/09 17:46:24 by abaudot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME	:= $(shell uname)

CC		=	gcc

CFLAGS		=	-Wall -Wextra -Werror -Ofast
IFLAGS		=	-I./inc
SRC_DIR		=	src
BUILD 		:=	.build
OBJ_DIR 	:=	$(BUILD)/obj

ifeq ($(UNAME), Linux)
	CFLAGS += -pthread
endif

NAME 		= philo

SRC 	:=	output.c				\
	  		init_philo_table.c		\
	  		lib.c					\
	  		actions.c				\
	  		philosopher.c

OBJ			:= $(SRC:%.c=$(OBJ_DIR)/%.o)

# COLORS

NONE			= \033[0m
CL_LINE			= \033[2K
S_CURSUS		= \033[s
J_CURSUS		= \033[u
B_BLACK			= \033[1;30m
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m
B_YELLOW		= \033[1;33m
B_BLUE			= \033[1;34m
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

#PRECOMPILE = mkdir -p $(dir $(OBJS_DIR)/$*)

$(BUILD):
	@mkdir $@ $(OBJ_DIR);

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(BUILD)
	@printf "$(CL_LINE)$(B_GREEN)Compiling srcs object : $(B_CYAN)$< $(NONE)..."
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@printf "$(CL_LINE)"
	@echo "[1 / 1] - $(B_MAGENTA)$@$(NONE)"
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJ) -o $@
	@echo "$(B_GREEN)Compilation done !$(NONE)"

all: $(NAME)

clean:
	@rm -Rf $(BUILD)
	@echo "$(B_RED)$(OBJ_DIR)$(NONE) -> $(B_GREEN)Delete$(NONE)"
	@echo "$(B_RED)$(OBJ_DIR_B)$(NONE) -> $(B_GREEN)Delete$(NONE)"

fclean: clean
	@rm -Rf $(NAME)
	@echo "$(B_RED)$(NAME)$(NONE)  -> $(B_GREEN)Delete$(NONE)"

re: fclean all

.PHONY: all clean fclean re
