/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/12 15:41:43 by fcrocq           ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	char	*input;
	t_token	*token_list;
	t_cmd	*cmd;
	t_env	*env_list;
	t_env	*original_env;
	char	*fixed_input;
	int		last_cmd_code;

	(void)ac;
	(void)av;
	cmd = NULL;
	token_list = NULL;
	if (!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	original_env = init_env(envp);
	if (!original_env)
	{
		original_env = init_minimal_env();
		if (!original_env)
			return (print_error_message("Error: couldn't create minimal env\n"));
	}
	env_list = copy_env_list(original_env);
	if (!env_list)
	{
		//free_env_list(env_list);
		return (print_error_message("Error: invalid env variable\n"));
	}
	free_env_list(&original_env);
	if (!env_list)
	{
		free_env_list(&original_env);
		return (print_error_message("Error: env variable init\n"));
	}
	env_list = change_var_value(env_list, "OLDPWD", pwd);
	last_cmd_code = 0;
	while (1)
	{
		handle_signals(SIGINT, PROMPT);
		handle_signals(SIGQUIT, IGNORE);
		input = prompt();
		if (!input)
			break ;
		fixed_input = fix_dollar_quote(input);
		free(input);
		if (!fixed_input)
			continue ;
		input = fixed_input;
		if (fixed_input)
		{
			if (handle_direct_env_var(fixed_input, env_list))
			{
				free(fixed_input);
				continue;
			}
		}
		token_list = parse_input(input, &last_cmd_code);
		if (!token_list)
		{
			free(input);
			input = NULL;
			continue ;
		}
		expand_tokens(token_list, env_list, &last_cmd_code);
		cmd = parse_commands(token_list, env_list, &last_cmd_code);
		free_token_list(token_list);
		free(input);
		input = NULL;
		close_all_fd(3);
		if (cmd)
		{
			handle_signals(SIGINT, IGNORE);
			if (cmd && handle_all_heredocs(cmd, env_list, &last_cmd_code) == -1)
			{
				free_commands(cmd);
				continue ;
			}
			if (is_empty_command(cmd))
			{
				if (is_redirection(cmd))
				{
					execute_only_redirections(cmd);
					free_commands(cmd);
					cmd = NULL;
					continue ;
				}
			}
			else if (is_builtins(cmd->args[0]) && !has_pipes(cmd))
			{
				if (is_redirection(cmd))
					handle_builtin_redirection(cmd, &env_list);
				else
					builtins_execution(cmd, &env_list);
			}
			else if (has_pipes(cmd))
				execute_pipeline(cmd, env_list);
			else if (is_redirection(cmd) && cmd->out && check_output_directory(cmd))
			{
				free_commands(cmd);
				cmd = NULL;
				continue ;
			}
			else if (is_redirection(cmd) && cmd->in && access(cmd->in, F_OK) == -1)
			{
				printf(ERR_DIR, cmd->in);
				free_commands(cmd);
				cmd = NULL;
				continue ;
			}
			else if (is_redirection(cmd))
				execute_redirection(cmd, env_list);
			else
				execute_commands(cmd, env_list);

		}
		if (cmd && cmd->exit_status)
		{
			printf("DEBUG MAIN: Récupération du code de sortie: %d\n", *(cmd->exit_status));
			last_cmd_code = *(cmd->exit_status);
		}
		else
			printf("DEBUG MAIN: Impossible de récupérer le code de sortie (pointeur NULL)\n");
		close_all_fd(3);
		if (cmd)
		{
			free_commands(cmd);
			cmd = NULL;
		}
	}
	if (input)
		free(input);
	free_env_list(&env_list);
	clear_history();
	close_all_fd(3);
	return (EXIT_SUCCESS);
}
