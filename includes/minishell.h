/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/12 09:37:39 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE //pour sigaction dans vscode

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
# define TOKEN_OPEN_PARENT 		10
# define TOKEN_CLOSE_PARENT 	11
# define TOKEN_AND

/*--------------- DEFINES COLORS --------------*/

# define BLACKB "\001\033[1;30m"
# define REDB "\001\033[1;31m"
# define GREENB "\001\033[1;32m"
# define YELLOWB "\001\033[1;33m"
# define BLUEB "\001\033[1;34m"
# define MAGENTAB "\001\033[1;35m"
# define CYANB "\001\033[1;36m"
# define WHITEB "\001\033[1;37m"
# define RESET "\001\033[0m"

/*--------------- DEFINES ERRORS --------------*/

# define ERR_NONE "\n"
# define ERR_SYNTAX "minislay: syntax error near unexpected token\n"
# define ERR_CMD "minislay: %s: command not found\n"
# define ERR_DIR "minislay: %s: No such file or directory\n"
# define ERR_ARG "minislay: %s: too many arguments\n"
# define ERR_EXP "minislay: export: %s: not a valid identifier\n"

/*--------------- DEFINES FOR SIGNALS --------------*/

# define IGNORE 0
# define DEFAULT 1
# define PROMPT 2
# define CHILD_PROMPT 3
# define CLOSE_IN 4

/*-------------- LIBRARIES --------------*/

# include <limits.h>
//# include <linux/limits.h>
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

struct s_cmd;
typedef struct s_cmd t_cmd;

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
	int				processed;
	int				*exit_status;
	pid_t			pid;
}	t_cmd;

typedef struct s_expand
{
	char	*result;
	size_t	capacity;
	size_t	i;
	size_t	j;
	t_env	*env_list;
	char	*str;
	t_cmd	*cmd;
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
	t_cmd	*cmd; // liste des commands
	t_env	**env_list; // variables d'environnemt
	int		input_fd; // entree
	int		pipe_fd[2]; // descripteur pipe
}	t_pipe;

/*-------------- FUNCTIONS --------------*/
void close_all_fd(int fd);
t_cmd	*get_last_cmd(t_cmd *cmd);
int size_list(t_token *token);
void	free_split(char **split);
int is_empty_command(t_cmd *cmd);
void execute_only_redirections(t_cmd *cmd);
void debug_exit_status(t_cmd *cmd);
int	handle_output_redirection(t_cmd *cmd);
void execute_with_heredoc(t_cmd *cmd, t_env *env_list);
int has_heredoc(t_cmd *cmd);
int handle_input_redirection(t_cmd *cmd);
void handle_builtin_redirection(t_cmd *cmd, t_env **env_list);
void free_env_array(char **env_array);
char **env_list_to_array(t_env *env_list);
int	is_valid_identifier(char *name);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	print_sorted_env(t_env *env_list);
t_env	*no_args_export(t_env *env_list);
int	is_valid_identifier(char *name);
char	*handle_char(char *merged, char current, int *quotes, char *quote_char);
int	is_escaped_char(char c);
int	process_variable_part1(t_expand *exp);
int	process_variable_part2(t_expand *exp);
int	process_variable_part3(t_expand *exp);
int	process_variable_part4(t_expand *exp);
int	process_variable_part5(t_expand *exp, int *code);
int	process_variable_part6(t_expand *exp);
char	*create_final_word(t_lexer *lexer, char *word, int start, int end);
int	check_quote_errors(t_lexer *lexer, char *word, int end);
int	handle_quote_case(char *input, char *result, int *index, int *quotes);
int	handle_dollar_case(char *input, char *result, int *index, int sq);
int	handle_edge_quotes(char *input, char *result, int *index);
int has_heredoc(t_cmd *cmd);
void execute_with_heredoc(t_cmd *cmd, t_env *env_list);
void	free_pipe_redir(t_cmd *cmd);
int setup_pipe(t_pipe *pipe_data);
void    in_redirection(t_cmd *current, int input_fd);
void    out_redirection(t_cmd *current, t_pipe *pipe_data);
void    pipe_builtin(t_cmd *current, t_pipe *pipe_data);
void    pipe_execve(t_cmd *current, t_pipe *pipe_data);
void	free_pipe(t_cmd *cmd, t_env *env_list, char **env);
void	redir_heredoc(int heredoc_fd);
void	redir_input(char *input_file);
void redir_output(char *output_file, int append_mode);
void	redir_execute(t_cmd *cmd, t_env *env_list);
void handle_pipe_redirection(t_cmd *cmd, t_env *env_list);
void	execute_redirection(t_cmd *cmd, t_env *env_list);
int	handle_input_redirection(t_cmd *cmd);
int	handle_all_heredocs(t_cmd *cmd, t_env *env_list, int *code);
int	handle_direct_env_var(char *input, t_env *env_list);
int	check_output_directory(t_cmd *commands);
t_env   *init_minimal_env(void);
long long	ft_atoll(const char *str, int *error);

/* BUILTINS */

t_env	*ft_cd(t_cmd *cmd, t_env *env_list);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_env *env_list);
void	ft_exit(t_cmd *cmd);
t_env	*ft_export(t_env *env_list, char *arg);
void	ft_pwd(t_cmd *cmd);
t_env	*ft_unset(t_env *env_list, char *name);

/* EXEC */

int		is_builtins(char *cmd);
void	builtins_execution(t_cmd *cmd, t_env **env_list);
char	*build_pathname(char *directory, char *arg);
char	*find_binary_path(char *arg, t_env *env_list);
void	execute_commands(t_cmd *cmd, t_env *env_list);
//void	execute_pipeline_cmd(t_cmd *cmd, t_env *env_list);
void	execute_child(t_cmd *cmd, int pipefd[2], t_env *env_list);
void	execute_parent_pipeline(t_cmd *cmd, int fd[2], pid_t pid, t_env *env);
void	execute_pipeline(t_cmd *cmd, t_env *env_list);
void	handle_pipe_error(int pipefd[2]);
int		create_pipe(int pipefd[2]);
pid_t	create_process(void);
int		has_pipes(t_cmd *cmd);
pid_t	create_pipe_and_fork(int pipefd[2]);
void	execute_redir_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, t_env *env);
void	execute_redirection(t_cmd *cmd, t_env *env_list);
void	handle_pipe_redirect(int pipefd[2], int mode, int *stdin_save);

// void	redirect_pipe(int input_fd, int pipe_fd[2], t_cmd *current);
// void	handle_child_pipe(t_cmd *current, int inpsdcsdcut_fd, int pipe_fd[2], t_env *env);
// void	create_execute_pipe(t_cmd *current, int *input_fd, int pipe_fd[2], t_env *env);
// void	execute_pipeline_cmd(t_cmd *current, t_env *env_list);
// void	handle_parent_pipe(pid_t pid, int *input_fd, int pipe_fd[2], t_cmd *current);
// void	handle_fork(t_cmd *current, int pipe_fd[2]);
// void	handle_pipe(int pipe_fd[2]);

/* EXPAND */

t_env	*create_env_element(char *env);
t_env	*init_env(char **envp);
t_env	*copy_env_list(t_env *original_env);
char	*extract_variable_name(t_expand *exp, size_t *len);
int		copy_variable_value(t_expand *exp, char *value, char *name);
int		process_variable(t_expand *exp, int *code);
int		resize_result_buffer(t_expand *exp);
int		check_buffer_size(t_expand *exp);
char	*expand_variable(t_env *env_list, char *str, int quote_type, int *code);
char	*init_expand_result(const char *str, t_expand *exp);
int		expand_loop(t_expand *exp, int *code);
void	expand_variable_in_token(t_token *token, t_env *env_list, int *code);
int		copy_str_to_result(t_expand *exp, char *str, int len);
void	expand_tokens(t_token *token_list, t_env *env_list, int *code);


/* LEXER - ok*/

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

/* PARSING - OK */

int		handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head, t_env *env_list, int *code);
int		init_args(t_cmd *cmd);
int		add_args(t_token *token, t_cmd *cmd);
t_cmd	*init_command(void);
t_cmd	*parse_commands(t_token *token_lis, t_env *env_list, int *code);
int		handle_redirection(t_token *token, t_cmd *current, t_cmd *head);
int		process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env);
int		get_token_type(char *token, int *command);
int		handle_std_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env);
int		process_other_token(t_token **token, t_cmd **curr, t_cmd **head, t_env *env);
int		process_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env);
int		process_pipe_token(t_token **token, t_cmd **current, t_cmd **head);




void	free_token_list(t_token *head);
int		open_file(char *filename, int token);

/* UTILS */

char	*find_var_value(t_env *env_list, char *name);
t_env	*change_var_value(t_env *env_list, char *name, char *value);
void	free_env_list(t_env **env_list);
void	free_elements(t_env *element);
void	free_commands(t_cmd *cmd);
void	free_tokens(char **tokens);
int		print_error_message(char *str);
void	quit_minislay(char *line, t_cmd *cmd, t_token *token, t_env *env);
void	clean_exit(t_token *tokens, char *input, t_cmd *commands);
char	*ft_realloc(char *str, size_t size);
int		is_redirection(t_cmd *cmd);
int		is_separator(int c);
int		is_space(int c);
void	skip_space(t_lexer *lexer);
int		is_numeric(char *str);

/* SIGNALS */

int		handle_signals(int sig, int param);
void	new_prompt(int sig);
void	child_new_prompt(int sig);
void	close_stdin(int sig);

void check_open_fds(void);

#endif
