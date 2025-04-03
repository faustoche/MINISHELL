/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/03 19:11:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*prompt(void)
{
	char	*line;

	line = readline("minislay> \033[1;32m→\033[0m \033[1;34m~\033[0m ");
	if (line && *line)
		add_history(line);
	return (line);
}

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;
	t_env	*env_list;
	t_env	*original_env;

	(void)ac;
	(void)av;
	commands = NULL;
	token_list = NULL;
	if(!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	original_env = init_env(envp);
	env_list = copy_env_list(original_env);
	if (!env_list)
	{
		return (print_error_message("Error: invalid env variable\n"));
	}
	free_env_list(&original_env);
	if (!env_list)
	{
		free_env_list(&original_env);
		return (print_error_message("Error: env variable init\n"));
	}
	env_list = change_var_value(env_list, "OLDPWD", pwd);
	while (1)
	{
		check_signals();
		if (token_list != NULL)
			free_token_list(token_list);
		input = prompt();
		if (!input)
			break ;
		token_list = parse_input(input);
		if (!token_list)
		{
			free(input);
			free_token_list(token_list);
			free_commands(commands);
			continue ;
		}
		expand_tokens(token_list, env_list);
		commands = parse_commands(token_list, env_list);
		free_token_list(token_list);
		close_all_fd(3);
		if (commands)
		{
			if (is_builtins(commands->args[0]) && !has_pipes(commands) && is_redirection(commands))
				builtins_execution(commands, &env_list);
			else if (has_pipes(commands))
				execute_pipeline(commands, env_list);
			else if (!is_redirection(commands))
				execute_redirection(commands, env_list);
			else
				execute_commands(commands, env_list);
		}
		close_all_fd(3);
		free_commands(commands);
		commands = NULL;
		token_list = NULL;
		free(input);
		input = NULL;
	}
	free_env_list(&env_list);
	clear_history();
	close_all_fd(3);
	return (0);
}
