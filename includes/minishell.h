/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 21:48:47 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE //pour sigaction dans vscode

/*--------------- DEFINES TOKENS --------------*/

# define TOKEN_COMMAND 	1
# define TOKEN_ARGUMENT 2
# define TOKEN_PIPE 	3
# define REDIR_OUT 		4
# define REDIR_IN 		5 
# define REDIR_APPEND 	6
# define TOKEN_SEP 		7
# define HEREDOC 		8
# define SINGLE_QUOTE	9

/*--------------- DEFINES COLORS --------------*/

# define BLACKB			"\001\033[1;30m"
# define REDB			"\001\033[1;31m"
# define GREENB			"\001\033[1;32m"
# define YELLOWB		"\001\033[1;33m"
# define BLUEB			"\001\033[1;34m"
# define MAGENTAB		"\001\033[1;35m"
# define CYANB			"\001\033[1;36m"
# define WHITEB			"\001\033[1;37m"
# define RESET			"\001\033[0m"

/*--------------- DEFINES ERRORS --------------*/

# define ERR_SYNTAX		"minislay: syntax error near unexpected token\n"
# define ERR_CMD		"minislay: %s: command not found\n"
# define ERR_DIR		"minislay: %s: No such file or directory\n"
# define ERR_ARG		"minislay: %s: too many arguments\n"
# define ERR_EXP		"minislay: export: %s: not a valid identifier\n"
# define ERR_HERE		"minislay: warning: here-document delimited by eof\n"

/*--------------- DEFINES FOR SIGNALS --------------*/

# define IGNORE			0
# define DEFAULT		1
# define PROMPT			2
# define CHILD_PROMPT	3
# define CLOSE_IN		4

/*-------------- LIBRARIES --------------*/

# include <limits.h>
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

/*------------- VARIABLE GLOBALE --------------*/

extern volatile sig_atomic_t	g_received_signal;

/*------------- STRUCTURES --------------*/

struct			s_cmd;
typedef struct	s_cmd t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	t_cmd			*cmd;
}	t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	t_cmd			*exit_status;
	int				quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	char			*in;
	char			*out;
	int				append;
	int				heredoc;
	char			*heredoc_eof;
	size_t			max_arg;
	size_t			nb_arg;
	struct s_cmd	*next;
	t_env			*env_list;
	char			**all_outputs;
	int				*exit_status;
	pid_t			pid;
}	t_cmd;

typedef struct s_expand
{
	char			*result;
	size_t			capacity;
	size_t			i;
	size_t			j;
	t_env			*env_list;
	char			*str;
	t_cmd			*cmd;
	int				quote_type;
}	t_expand;

typedef struct s_lexer
{
	int				pos;
	int				command;
	t_cmd			*cmd;
	char			*input;
	t_token			*tokens;
	int				*exit_code;
}	t_lexer;

typedef struct s_state
{
	int				i;
	int				j;
	int				singles;
	int				doubles;
}	t_state;

typedef struct s_pipe
{
	t_cmd			*cmd;
	t_env			**env_list;
	int				input_fd;
	int				pipe_fd[2];
}	t_pipe;

/*-------------- FUNCTIONS --------------*/

/* BUILTINS */

void	ft_pwd_cd(void);
t_env	*ft_cd(t_cmd *cmd, t_env *env_list);
int		cd_errors(t_cmd *cmd, char *dir, t_env **new_env);
int		cd_no_home(char *home);
int		cd_no_arg(char *home, char *new_dir);
t_env	*cd_update_env(t_cmd *cmd, t_env *new_env_list, char *old_pwd, char *new_dir);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_env *env_list);
void	print_sorted_env(t_env *env_list);
void	ft_exit(t_cmd *cmd);
t_env	*ft_export(t_env *env_list, char *arg, int *code);
int		is_valid_identifier(char *name);
t_env	*no_args_export(t_env *env_list);
void	ft_pwd(t_cmd *cmd);
t_env	*ft_unset(t_env *env_list, char *name);

/* ENV */

char	*find_var_value(t_env *env_list, char *name);
t_env	*copy_env_list(t_env *original_env);
t_env	*init_minimal_env(void);
t_env	*create_env_element(char *env);
t_env	*init_env(char **envp);
t_env	*change_var_value(t_env *env_list, char *name, char *value);

/* EXEC */

int		is_builtins(char *cmd);
void	builtins_execution(t_cmd *cmd, t_env **env_list);
char	*build_pathname(char *directory, char *arg);
char	*find_bin_path(char *arg, t_env *env_list);
void	execute_commands(t_cmd *cmd, t_env *env_list);
t_cmd	*get_last_cmd(t_cmd *cmd);
void	execute_pipeline(t_cmd *cmd, t_env *env_list);
int		setup_pipe(t_pipe *pipe_data);
void	in_redirection(t_cmd *current, int input_fd);
void	out_redirection(t_cmd *current, t_pipe *pipe_data);
void	pipe_builtin(t_cmd *current, t_pipe *pipe_data);
void	pipe_execve(t_cmd *current, t_pipe *pipe_data);
char	**env_list_to_array(t_env *env_list);
int		handle_input_redirection(t_cmd *cmd);
int		handle_output_redirection(t_cmd *cmd);
void	handle_builtin_redirection(t_cmd *cmd, t_env **env_list);
void	free_pipe(t_cmd *cmd, t_env *env_list, char **env);
void	free_pipe_redir(t_cmd *cmd);
void	redir_heredoc(int heredoc_fd);
void	redir_input(char *input_file, int *code);
void	redir_output(char *output_file, int append_mode, int *code);
int		redir_execute(t_cmd *cmd, t_env *env_list);
t_pipe	init_pipe_struct(t_cmd *cmd, t_env **env_list);
void	pipe_child_process(t_cmd *current, t_pipe *pipe_data);
void	pipe_parent_process(t_cmd **current, t_pipe *pipe_data);
void	handle_pipe_redirection(t_cmd *cmd, t_env *env_list);
void	handle_child_process(t_cmd *cmd, t_env *env, int pipefd[2], int fd);
void	handle_exit_status_signals(int status, t_cmd *cmd);
void	execute_redirection(t_cmd *cmd, t_env *env_list);
int		execute_only_redirections(t_cmd *cmd);
pid_t	create_pipe_and_fork(int pipefd[2]);
void	execute_redir_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, t_env *env);
void	handle_pipe_redirect(int pipefd[2], int mode, int *stdin_save);
int		check_output_directory(t_cmd *commands);

/* EXPAND */

char	*expand_variable(t_env *env_list, char *str, int quote_type, int *code);
char	*init_expand_result(const char *str, t_expand *exp);
int		expand_loop(t_expand *exp, int *code);
void	expand_variable_in_token(t_token *token, t_env *env_list, int *code);
void	expand_tokens(t_token *token_list, t_env *env_list, int *code);
int		process_variable_part6(t_expand *exp);
char	*extract_variable_name(t_expand *exp, size_t *len);
int		copy_variable_value(t_expand *exp, char *value, char *name);
int		resize_result_buffer(t_expand *exp);
int		check_buffer_size(t_expand *exp);
int		copy_str_to_result(t_expand *exp, char *str, int len);
int		process_variable(t_expand *exp, int *code);
int		process_variable_part1(t_expand *exp);
int		process_variable_part2(t_expand *exp);
int		process_variable_part3(t_expand *exp);
int		process_variable_part4(t_expand *exp);
int		process_variable_part5(t_expand *exp, int *code);

/* LEXER */

t_token	*parse_input(char *input, int *code);
t_token	*lexing(char *input);
int		handle_mixed_quotes(t_lexer *lexer, int start);
char	*fix_dollar_quote(char *input);
int		is_dollar_quote(char *input, int i);
int		handle_dollar_quote(char *input, char *result, int i, int *j);
int		handle_quote_state(int state);
int		handle_dollar_dq(char *input, char *result, int i, int *j);
void	handle_outside_quotes(char *input, char *processed, t_state *state);
int		handle_dollar_case(char *input, char *result, int *index, int sq);
int		handle_quote_case(char *input, char *result, int *index, int *quotes);
int		handle_edge_quotes(char *input, char *result, int *index);
int		handle_delimiter(t_lexer *lexer, int i);
char	*handle_escape_char(char *input);
int		syntax_error(char *input);
int		delimiter_error(char *input);
int		character_error(char *input);
int		input_check(char *input);
int		handle_word(t_lexer *lexer, int start);
int		handle_special_char(t_lexer *lexer);
void	add_token(t_lexer *lexer, char *word, int length, int type);
char	*handle_char(char *merged, char current, int *quotes, char *quote_char);
int		check_quote_errors(t_lexer *lexer, char *word, int end);
char	*create_final_word(t_lexer *lexer, char *word, int start, int end);

/* PARSING */

int		handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head);
int		handle_all_heredocs(t_cmd *cmd);
int		init_args(t_cmd *cmd);
int		add_args(t_token *token, t_cmd *cmd);
t_cmd	*init_command(void);
char	*remove_quotes(char *str);
t_cmd	*parse_commands(t_token *token_lis, t_env *env_list, int *code);
int		process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env);
int		handle_redirection(t_token *token, t_cmd *current, t_cmd *head);
int		get_token_type(char *token, int *command);
int		handle_std_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env);
int		process_other_token(t_token **token, t_cmd **curr, t_cmd **head, t_env *env);
int		process_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env);
int		process_pipe_token(t_token **token, t_cmd **current, t_cmd **head);

/* SIGNALS */

int		handle_signals(int sig, int param);
void	new_prompt(int sig);
void	child_new_prompt(int sig);
void	close_stdin(int sig);

/* UTILS */

int		is_escaped_char(char c);
int		has_pipes(t_cmd *cmd);
int		is_empty_command(t_cmd *cmd);
void	quit_minislay(char *line, t_cmd *cmd, t_token *token, t_env *env);
void	clean_exit(t_token *tokens, char *input, t_cmd *commands);
int		print_error_message(char *str);
void	close_all_fd(int fd);
int		open_file(char *filename, int token, int *code);
void	ft_memdel(void *ptr);
void	free_commands(t_cmd *cmd);
char	*ft_realloc(char *str, size_t size);
void	free_split(char **split);
void	free_env_array(char **env_array);
void	free_env_list(t_env **env_list);
void	free_elements(t_env *element);
void	free_token_list(t_token *head);
void	free_pipe_redir(t_cmd *cmd);
void	ft_lstadd_back(t_env **lst, t_env *new);
int		handle_direct_env_var(char *input, t_env *env_list);
int		is_separator(int c);
int		is_space(int c);
void	skip_space(t_lexer *lexer);
int		is_redirection(t_cmd *cmd);
int		is_numeric(char *str);

#endif
