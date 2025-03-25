/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/25 14:16:01 by faustoche        ###   ########.fr       */
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
# define SINGLE_QUOTE			9
# define DOUBLE_QUOTE			10
# define TOKEN_EXPORT_KEY 		11
# define TOKEN_EXPORT_VALUE 	12
# define TOKEN_OPEN_PARENT 		13
# define TOKEN_CLOSE_PARENT 	14
# define TOKEN_AND 				15
# define TOKEN_OR				16
# define TOKEN_ENV_VAR			17

/*--------------- DEFINES ERRORS --------------*/

# define ERR_NONE "\n"
# define ERR_SYNTAX "bash: syntax error near unexpected token\n"
# define ERR_CMD "bash: %s: command not found\n"
# define ERR_DIR "bash: %s: is a directory\n"
# define ERR_ARG "bash: %s: too many arguments\n"
# define ERR_INVALID_TOK

/*-------------- LIBRARIES --------------*/

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>			
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../src/get_next_line/get_next_line.h"
# include "../libft/libft.h"

/*------------- STRUCTURES --------------*/

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef	struct s_expand
{
	char	*result;
	size_t	capacity;
	size_t	i;
	size_t	j;
	t_env	*env_list;
	char	*str;
}	t_expand;

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
	int				heredoc;
	size_t			max_arg;
	size_t			nb_arg;
	struct s_cmd	*next;
	t_env			*env_list;
	int				exit_status;
}	t_cmd;

typedef	struct s_lexer
{
	int				pos;
	char			*input;
	t_token			*tokens;
	int				command;
}	t_lexer;

/*-------------- FUNCTIONS --------------*/

/* BONUSES */

int		match_wildcard(char *sign, char *name);
char	**init_matches(void);
void	clean_matches(char **matches, int count, DIR *dir);
int		add_match(char **matches, int count, char *name);
char	**handle_no_matches(char *sign);
int 	collect_matches(char **matches, DIR *dir, char *sign);
void	free_wildcards(char **matches);
char	**expand_wildcards(char *sign);

/* BUILTINS */

void 	ft_cd(t_cmd *cmd);
void	ft_echo(t_cmd *cmd);
void    ft_env(t_env *env_list);
void	ft_exit(t_cmd *cmd);
t_env	*find_env_var(t_env *env_list, char *name);
void	add_env_var(t_env **env_list, char *name, char *value);
void	export_variable(t_env **env_list, char *arg);

/* EXEC */

int 	is_builtins(char *cmd);
int		builtins_execution(t_cmd *cmd, t_env *env_list);
int		builtins_execution2(t_cmd *cmd, t_env *env_list);
char	*build_pathname(char *directory, char *arg);
char	*find_binary_path(char *arg);
void	execute_commands(t_cmd *cmd, t_env *env_list);
void	execute_pipeline_cmd(t_cmd *cmd, t_env *env_list);
void	execute_child(t_cmd *cmd, int pipefd[2], t_env *env_list);
void 	execute_parent_pipeline(t_cmd *cmd, int pipefd[2], pid_t pid, t_env *env_list);
void 	execute_pipeline(t_cmd *cmd, t_env *env_list);
void	handle_pipe_error(int pipefd[2]);
int 	create_pipe(int pipefd[2]);
pid_t 	create_process();
int 	has_pipes(t_cmd *cmd);
pid_t	create_pipe_and_fork(int pipefd[2]);
void 	execute_redirect_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, int *stdin_save, t_env *env_list);
void 	execute_redirection(t_cmd *cmd, t_env *env_list);
void	handle_pipe(int pipefd[2], int mode, int *stdin_save);

/* EXPAND */

char	*extract_variable_name(t_expand *exp, size_t *len);
int		copy_variable_value(t_expand *exp, char *value, char *name);
int		process_variable(t_expand *exp);
int		resize_result_buffer(t_expand *exp);
int		check_buffer_size(t_expand *exp);
t_env	*create_env_element(char *env);
char	*get_env_value(t_env *env_list, char *name);
t_env	*init_env(char **envp);
char	*expand_variable(t_env *env_list, char *str, int quote_type);
char	*init_expand_result(const char *str, t_expand *exp);
int		expand_loop(t_expand *exp);
void	expand_variable_in_token(t_token *token, t_env *env_list);
void	add_wildcard_tokens(t_lexer *lexer, char **matches, int start_index);
void	expand_wildcard_in_token(t_token *token, t_lexer *lexer);
void	expand_tokens(t_token *token_list, t_env *env_list);

/* LEXER */

int		syntax_error(char *input);
int		delimiter_error(char *input);
int		character_error(char *input);
int		input_check(char *input);
int 	single_quotes(t_lexer *lexer, int i);
int 	double_quotes(t_lexer *lexer, int i);
int		double_delimiter(char *input, int i);
int 	handle_delimiter(t_lexer *lexer, int i);
t_token	*lexing(char *input);
int		handle_special_char(t_lexer *lexer);
void	add_token(t_lexer *lexer, char *word, int length, int type);
int 	handle_word(t_lexer *lexer, int start);

/* PARSING */

int		init_args(t_cmd *cmd);
int		expand_args(t_cmd *cmd);
int		add_args(t_token *token, t_cmd *cmd);
t_cmd	*init_command(void);
t_cmd	*parse_commands(t_token *token_lis, t_env *env_list);
int		process_pipe_token(t_token **token, t_cmd **current, t_cmd **head);
int		process_redirection_token(t_token **token, t_cmd **current, t_cmd **head);
int		process_other_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env_list);
int		process_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env_list);
int		redirection_token(t_token *token);
int		redirection_process(t_token **token, t_cmd **current, t_cmd **head);
void	free_token_list(t_token *head);
t_token	*parse_input(char *input);
int		open_file(char *filename, int token);
void	redirect(int fd, int std_fd);
int		handle_redirection(t_token *token, t_cmd *current, t_cmd *head);
int		redirection(t_cmd *cmd, char *file, int out, int append);
int		handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head);
int		get_token_type(char *token, int *command);
int		handle_standard_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env_list);

/* UTILS */

void 	free_env_list(t_env *env_list);
void	free_elements(t_env *element);
void	free_commands(t_cmd *cmd);
void	free_tokens(char **tokens);
void	quit_minislay(char *line, t_cmd *cmd, t_token *token);
void 	clean_exit(t_token *tokens, char *input, t_cmd *commands);
char	*ft_realloc(char *str, size_t size);
int		is_redirection(t_cmd *cmd);
int		is_separator(int c);
int		is_space(int c);
void	skip_space(t_lexer *lexer);
int 	is_numeric(char *str);

#endif
