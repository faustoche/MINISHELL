# ************************************************************************ #
#                			        MAIN                                   #
# ************************************************************************ #

NAME 		= minishell


# ************************************************************************ #
#                			     DIRECTORIES                               #
# ************************************************************************ #

INCLUDES_MINI	= includes
INCLUDES_LIB	= libft/includes
LIBFT_DIR		= libft
SRCS_DIR		= src
OBJS_DIR		= obj

# ************************************************************************ #
#                			 SRCS && OBJECTS FILES                         #
# ************************************************************************ #

MINISHELL = 	$(addprefix $(SRCS_DIR)/, main.c\
				main_loop.c\
				builtins/ft_cd1.c\
				builtins/ft_cd2.c\
				builtins/ft_echo.c\
				builtins/ft_env.c\
				builtins/ft_exit.c\
				builtins/ft_export1.c\
				builtins/ft_export2.c\
				builtins/ft_pwd.c\
				builtins/ft_unset.c\
				env/env_copy.c\
				env/env_init.c\
				env/env_creation1.c\
				env/env_creation2.c\
				exec/exec_cmd.c\
				exec/exec_cmd2.c\
				exec/exec_cmd3.c\
				exec/exec_pipe1.c\
				exec/exec_pipe2.c\
				exec/exec_pipe3.c\
				exec/exec_redir1.c\
				exec/exec_redir2.c\
				exec/exec_redir3.c\
				exec/exec_redir4.c\
				exec/exec_builtins.c\
				exec/exec_utils1.c\
				exec/exec_utils2.c\
				parsing/parse_heredoc.c\
				parsing/parse_init1.c\
				parsing/parse_init2.c\
				parsing/parse_token.c\
				parsing/parse_redir1.c\
				parsing/parse_redir2.c\
				lexer/lexer_init.c\
				lexer/lexer_quotes1.c\
				lexer/lexer_quotes2.c\
				lexer/lexer_quotes3.c\
				lexer/lexer_quotes4.c\
				lexer/lexer_separator.c\
				lexer/lexer_syntax.c\
				lexer/lexer_words1.c\
				lexer/lexer_words2.c\
				lexer/lexer_words3.c\
				expand/expand_init.c\
				expand/expand_token.c\
				expand/expand_utils.c\
				expand/expand_variable1.c\
				expand/expand_variable2.c\
				expand/expand_variable3.c\
				utils/utils_clean.c\
				utils/utils_files.c\
				utils/utils_free_cmd1.c\
				utils/utils_free_cmd2.c\
				utils/utils_free_list.c\
				utils/utils_parsing.c\
				utils/utils_others.c\
				utils/utils_check.c\
				signals/signals.c)

SRCS			= ${MINISHELL}
MINISHELL_OBJS	= ${MINISHELL:${SRCS_DIR}/%.c=$(OBJS_DIR)/%.o}
OBJS			= $(MINISHELL_OBJS)
LIBFT 			= -L$(LIBFT_DIR) -lft
# -l:libft.a

# ************************************************************************ #
#                    			 COMPILATION                               #
# ************************************************************************ #

CC          = cc
CFLAGS      = -Wall -Werror -Wextra -g3 -I$(INCLUDES_MINI) -I$(INCLUDES_LIB) -I/opt/homebrew/opt/readline/include
LDFLAGS     = -L/opt/homebrew/opt/readline/lib -lreadline
RM          = rm -rf


# ************************************************************************ #
#                  		       	 PROCESS                                   #
# ************************************************************************ #

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	@echo "\033[1;33m\n🪩  COMPILING MINISHELL... 🪩\n"
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBFT) -o $(NAME)
	@echo "\033[1;32m💾 ./$(NAME) created 💾\n"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)


# ************************************************************************ #
#                          LIBFT COMPILATION                               #
# ************************************************************************ #

$(LIBFT_DIR)/libft.a:
	@echo "\033[1;33m\n🔨 COMPILING LIBFT... 🔨\n"
	@$(MAKE) -C $(LIBFT_DIR) -s
	@echo "\033[1;32m💾 LIBFT COMPILED 💾\n"

# ************************************************************************ #
#                  		     CLEANUP SESSION                               #
# ************************************************************************ #

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) clean -C $(LIBFT_DIR) -s
	@echo "\033[1;32m🫧  DONE 🫧"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR) -s
	@echo "\033[1;32m🫧  DONE 🫧"

re: fclean all
	@echo "\033[1;32m🫧  RE DONE 🫧"

.PHONY: all clean fclean re

# ************************************************************************ #
#                  	        	     COLORS                                #
# ************************************************************************ #

COLOR_RESET = \033[0m

# Regular Colors
COLOR_BLACK = \033[0;30m
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_YELLOW = \033[0;33m
COLOR_BLUE = \033[0;34m
COLOR_MAGENTA = \033[0;35m
COLOR_CYAN = \033[0;36m
COLOR_WHITE = \033[0;37m

# Bright Colors
COLOR_BLACKB = \033[1;30m
COLOR_REDB = \033[1;31m
COLOR_GREENB = \033[1;32m
COLOR_YELLOWB = \033[1;33m
COLOR_BLUEB = \033[1;34m
COLOR_MAGENTAB = \033[1;35m
COLOR_CYANB = \033[1;36m
COLOR_WHITEB = \033[1;37m

# Additional Styles
COLOR_BOLD = \033[1m
COLOR_DIM = \033[2m
COLOR_ITALIC = \033[3m
COLOR_UNDERLINE = \033[4m
COLOR_BLINK = \033[5m
COLOR_REVERSE = \033[7m
COLOR_HIDDEN = \033[8m
