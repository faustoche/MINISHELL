/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 13:46:31 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* - volatile : force le compilateur a acceder a 
			la memoire ou la variable est stockee,
			pour etre sur que mise a jour bien effectuee
			dans gestionnaire de signaux.
	- sig_atomic_t : garantit que acces a la variable ne peuvent
			etre interrompus ou modifies de facon incomplete. 
*/

volatile sig_atomic_t	g_received_signal = 0;

static char	*prompt(void)
{
	char	*line;

	line = readline("minislay> \033[1;34m~\033[0m ");
	if (!line)
	{
		printf("exit\n");
		clear_history();
		free(line);
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

t_env	*init_environment(char **envp, char *pwd)
{
	t_env	*env_list;
	t_env	*og_env;

	og_env = init_env(envp);
	if (!og_env)
	{
		og_env = init_minimal_env();
		if (!og_env)
		{
			printf("Error: cannot create minimal env\n");
			return (NULL);
		}
	}
	env_list = copy_env_list(og_env);
	if (!env_list)
	{
		printf("Error: invalid env variable\n");
		return (NULL);
	}
	free_env_list(&og_env);
	if (!env_list)
	{
		free_env_list(&og_env);
		printf("Error: env variable init\n");
		return (NULL);
	}
	env_list = change_var_value(env_list, "OLDPWD", pwd);
	return (env_list);
}

void	handle_signals_main(int *last_cmd_code)
{
	handle_signals(SIGINT, PROMPT);
	handle_signals(SIGQUIT, IGNORE);
	if (g_received_signal == SIGINT)
	{
		*last_cmd_code = 128 + SIGINT;
		g_received_signal = 0;
	}
}

char	*first_input_check(int *last_cmd_code, t_env *env_list)
{
	char	*input;
	char	*fixed_input;

	input = prompt();
	if (!input)
		return (NULL);
	if (g_received_signal == SIGINT)
	{
		*last_cmd_code = 128 + SIGINT;
		g_received_signal = 0;
	}
	fixed_input = fix_dollar_quote(input);
	free(input);
	if (!fixed_input)
		return (NULL);
	//input = fixed_input;
	if (handle_direct_env_var(fixed_input, env_list))
	{
		free(fixed_input);
		return (NULL);
	}
	return (fixed_input);
}

t_cmd	*parse_and_expand(char *input, t_env *env_list, int *last_cmd_code)
{
	t_token	*token_list;
	t_cmd	*cmd;
	
	token_list = parse_input(input, last_cmd_code);
	if (!token_list)
	{
		free(input);
		input = NULL;
		return (NULL);
	}
	expand_tokens(token_list, env_list, last_cmd_code);
	cmd = parse_commands(token_list, env_list, last_cmd_code);
	free_token_list(token_list);
	close_all_fd(3);
	return (cmd);
}

int	before_execution(t_cmd *cmd)
{
	handle_signals(SIGINT, IGNORE);
	if (handle_all_heredocs(cmd) == -1)
		return (-1);
	return (0);
}

int	execute_flow(t_cmd *cmd, t_env **env_list)
{
	if (is_empty_command(cmd))
	{
		if (is_redirection(cmd))
			execute_only_redirections(cmd);
	}
	else if (is_builtins(cmd->args[0]) && !has_pipes(cmd))
	{
		if (is_redirection(cmd))
			handle_builtin_redirection(cmd, env_list);
		else
			builtins_execution(cmd, env_list);
	}
	else if (has_pipes(cmd))
		execute_pipeline(cmd, *env_list);
	else if (is_redirection(cmd) && cmd->out && check_output_directory(cmd))
		return (1);
	else if (is_redirection(cmd) && cmd->in && access(cmd->in, F_OK) == -1)
	{
		printf(ERR_DIR, cmd->in);
		return (1);
	}
	else if (is_redirection(cmd))
		execute_redirection(cmd, *env_list);
	else
		execute_commands(cmd, *env_list);
	return (0);
}

void	process_commands(char *input, t_env **env_list, int *last_cmd_code)
{
	t_cmd	*cmd;

	cmd = parse_and_expand(input, *env_list, last_cmd_code);
	if (!cmd)
		return ;
	if (before_execution(cmd) == -1)
	{
		if (cmd->exit_status)
			*last_cmd_code = *(cmd->exit_status);
		free_commands(cmd);
		return ;
	}
	if (execute_flow(cmd, env_list) == 1)
	{
		*last_cmd_code = 1;
		free_commands(cmd);
		return ;
	}
	if (cmd->exit_status)
		*last_cmd_code = *(cmd->exit_status);
	free_commands(cmd);
}

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	t_env	*env_list;
	char	*input;
	int		last_cmd_code = 0;

	(void)ac;
	(void)av;
	if (!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	env_list = init_environment(envp, pwd);
	if (!env_list)
		return (1);
	while (1)
	{
		handle_signals_main(&last_cmd_code);
		input = first_input_check(&last_cmd_code, env_list);
		if (!input)
			break ;
		process_commands(input, &env_list, &last_cmd_code);
		free(input);
	}
	free_env_list(&env_list);
	clear_history();
	close_all_fd(3);
	return (EXIT_SUCCESS);
}

// int	main(int ac, char **av, char **envp)
// {
// 	char	pwd[PATH_MAX];
// 	char	*input;
// 	char	*fixed_input;
// 	t_token	*token_list;
// 	t_cmd	*cmd;
// 	t_env	*env_list;
// 	t_env	*original_env;
// 	int		last_cmd_code;

// 	(void)ac;
// 	(void)av;
// 	cmd = NULL;
// 	token_list = NULL;
// 	if (!(getcwd(pwd, sizeof(pwd))))
// 		return (print_error_message("Error: can't get pwd\n"));
// 	original_env = init_env(envp);
// 	if (!original_env)
// 	{
// 		original_env = init_minimal_env();
// 		if (!original_env)
// 			return (print_error_message("Error: couldn't create minimal env\n"));
// 	}
// 	env_list = copy_env_list(original_env);
// 	if (!env_list)
// 		return (print_error_message("Error: invalid env variable\n"));
// 	free_env_list(&original_env);
// 	if (!env_list)
// 	{
// 		free_env_list(&original_env);
// 		return (print_error_message("Error: env variable init\n"));
// 	}
// 	env_list = change_var_value(env_list, "OLDPWD", pwd);
// 	last_cmd_code = 0;
// 	while (1)
// 	{
// 		handle_signals(SIGINT, PROMPT);
// 		handle_signals(SIGQUIT, IGNORE);
// 		if (g_received_signal == SIGINT)
// 		{
// 			last_cmd_code = 128 + SIGINT;
// 			g_received_signal = 0;
// 		}
// 		input = prompt();
// 		if (!input)
// 			break ;
// 		if (g_received_signal == SIGINT)
// 		{
// 			last_cmd_code = 128 + SIGINT;
// 			g_received_signal = 0;
// 		}
// 		fixed_input = fix_dollar_quote(input);
// 		free(input);
// 		if (!fixed_input)
// 			continue ;
// 		input = fixed_input;
// 		if (fixed_input)
// 		{
// 			if (handle_direct_env_var(fixed_input, env_list))
// 			{
// 				free(fixed_input);
// 				continue ;
// 			}
// 		}
// 		token_list = parse_input(input, &last_cmd_code);
// 		if (!token_list)
// 		{
// 			free(input);
// 			input = NULL;
// 			continue ;
// 		}
// 		expand_tokens(token_list, env_list, &last_cmd_code);
// 		cmd = parse_commands(token_list, env_list, &last_cmd_code);
// 		free_token_list(token_list);
// 		free(input);
// 		input = NULL;
// 		close_all_fd(3);
// 		if (cmd)
// 		{
// 			handle_signals(SIGINT, IGNORE);
// 			if (cmd && handle_all_heredocs(cmd) == -1)
// 			{
// 				if (cmd && cmd->exit_status)
// 					last_cmd_code = *(cmd->exit_status);
// 				free_commands(cmd);
// 				continue ;
// 			}
// 			if (is_empty_command(cmd))
// 			{
// 				if (is_redirection(cmd))
// 				{
// 					execute_only_redirections(cmd);
// 					if (cmd && cmd->exit_status)
// 						last_cmd_code = *(cmd->exit_status);
// 					free_commands(cmd);
// 					cmd = NULL;
// 					continue ;
// 				}
// 			}
// 			else if (is_builtins(cmd->args[0]) && !has_pipes(cmd))
// 			{
// 				if (is_redirection(cmd))
// 					handle_builtin_redirection(cmd, &env_list);
// 				else
// 					builtins_execution(cmd, &env_list);
// 			}
// 			else if (has_pipes(cmd))
// 				execute_pipeline(cmd, env_list);
// 			else if (is_redirection(cmd) && cmd->out && check_output_directory(cmd))
// 			{
// 				last_cmd_code = 1;
// 				free_commands(cmd);
// 				cmd = NULL;
// 				continue ;
// 			}
// 			else if (is_redirection(cmd) && cmd->in && access(cmd->in, F_OK) == -1)
// 			{
// 				last_cmd_code = 1;
// 				printf(ERR_DIR, cmd->in);
// 				free_commands(cmd);
// 				cmd = NULL;
// 				continue ;
// 			}
// 			else if (is_redirection(cmd))
// 				execute_redirection(cmd, env_list);
// 			else
// 				execute_commands(cmd, env_list);
// 		}
// 		if (cmd && cmd->exit_status)
// 			last_cmd_code = *(cmd->exit_status);
// 		if (cmd)
// 		{
// 			free_commands(cmd);
// 			cmd = NULL;
// 		}
// 	}
// 	if (input)
// 		free(input);
// 	free_env_list(&env_list);
// 	clear_history();
// 	close_all_fd(3);
// 	return (EXIT_SUCCESS);
// }
