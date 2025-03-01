/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/01 15:53:23 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*--------------- DEFINES TOKENS --------------*/

# define TOKEN_COMMAND 			1
# define TOKEN_ARGUMENT 		2
# define TOKEN_PIPE 			3
# define REDIR_OUT 				4
# define REDIR_IN 				5 
# define REDIR_APPEND 			6
# define TOKEN_SEPARATOR 		7
# define HEREDOC 				8
# define SINGLE_DEL				9
# define DOUBLE_DEL				10
# define TOKEN_EXPORT_KEY 		11
# define TOKEN_EXPORT_VALUE 	12
# define TOKEN_OPEN_PARENT 		13
# define TOKEN_CLOSE_PARENT 	14
# define TOKEN_AND 				15
# define TOKEN_OR				16

/*--------------- DEFINES ERRORS --------------*/

# define ERR_NONE "\n"
# define ERR_SYNTAX "bash: syntax error near unexpected token\n"
# define ERR_CMD "bash: %s: command not found\n"
# define ERR_DIR "bash: %s: is a directory\n"
# define ERR_INVALID_TOK

/*-------------- LIBRARIES --------------*/

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>			
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../src/get_next_line/get_next_line.h"

/*------------- STRUCTURES --------------*/

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	char			*in;
	char			*out;
	int				pipe;
	int				append;
	size_t			max_arg;
	size_t			nb_arg;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef	struct s_lexer
{
	int				pos;
	char			*input;
	t_token			*tokens;
	int				command;
}	t_lexer;

/*-------------- FUNCTIONS --------------*/

/* Main */

char	*prompt(void);
void	print_welcome_message();
int 	get_env(char *path);

/* Env */

t_env	*create_env_element(char *env);
t_env	*init_env(char **envp);
void 	free_env_list(t_env *env_list);
int		free_elements(t_env *element);

/* Lexer */

t_token	*lexing(char *input);
int 	single_quotes(t_lexer *lexer, int i);
int 	double_quotes(t_lexer *lexer, int i);
int		double_delimiter(char *input, int i);
int 	handle_delimiter(t_lexer *lexer, int i);
void	add_token(t_lexer *lexer, char *word, int length, int type);
int		add_word_token(t_lexer *lexer, int start, int end);
void	free_lexer(t_lexer *lexer);
int 	handle_word(t_lexer *lexer, int start);
int		syntax_error(char *input);
int		delimiter_error(char *input);
int		character_error(char *input);
int		string_error(t_token *token);

/* Parsing */

	// Parse arguments
int		init_args(t_cmd *command);
int		expand_args(t_cmd *command);
int		add_args(t_cmd *command, char *arg);

	// Parse command
t_cmd	*init_command(void);
t_cmd	*parse_commands(t_token *token_list);
int		process_token(t_token **token, t_cmd **current, t_cmd **head);
int		redirection_token(t_token *token);
int		redirection_process(t_token **token, t_cmd **current, t_cmd **head);

	// Parse input
void	free_token_list(t_token *head);
t_token	*parse_input(char *input);
void	print_tokens(t_token *head);

	// Parse token
int		get_token_type(char *token, int *command);
int		find_token(char *token, int *command);
int		handle_standard_token(t_cmd **current, t_cmd **head, char *value);

	// Redirection
int		open_file(char *filename, int token);
void	redirect_out(int fd);
void	redirect_in(int fd);
int		handle_redirection(t_cmd *current, t_token *token, t_cmd *head);
int		redirection(t_cmd *cmd, char *file, int out, int append);
int		handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head);

/* Utils */

	// Free
void	free_commands(t_cmd *cmd);
void	free_tokens(char **tokens);
void	quit_minislay(char *line, t_cmd *cmd, t_token *token);
void 	free_token_list(t_token *head);
void 	clean_exit(t_token *tokens, char *input, t_cmd *commands);

	// Split
char	**ft_split(char *input);
int		count_tokens(char *input);
int		is_separator(int c);
int		is_space(int c);
int		handle_space(char *input, int i);
int		handle_separator(char **tokens, int *count, char *input, int i);
void	input_to_tokens(char **tokens, int *count, char *start, int length);
int		handle_quotes(char **tokens, int *count, char *input, int i);
int		check_word(char **tokens, int *count, char *input, int i);

	// Utils
int		ft_strcmp(char *s1, char *s2);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
char	*ft_strdup(const char *s);
void	print_commands(t_cmd *cmd);
char	*ft_strndup(const char *s, size_t n);

/* Exec */

int		handle_pipe(t_cmd **current);

#endif