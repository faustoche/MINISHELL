/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/28 11:35:16 by fcrocq           ###   ########.fr       */
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
	//char	*pwd;
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;
	t_env	*env_list;

	(void)ac;
	(void)av;
	//pwd = getcwd(NULL, 0);
	env_list = copy_env_list(init_env(envp));
	if (!env_list)
		return (print_error_message("Error: env variable init\n"));
	//env_list = change_var_value(env_list, "OLDPWD", pwd);
	while (1)
	{
		input = prompt();
		if (!input)
		{
			printf("error\n");
			break ;
		}
		token_list = parse_input(input);
		if (!token_list)
		{
			free(input);
			continue ;
		}
		expand_tokens(token_list, env_list);
		commands = parse_commands(token_list, env_list);
		if (commands)
		{
			if (commands)
			{
				if (is_builtins(commands->args[0]) && !has_pipes(commands))
				{
					printf("builtins main");
					builtins_execution(commands, &env_list);
				}
				else if (has_pipes(commands))
				{
					printf("execute pipeline main\n");
					execute_pipeline(commands, env_list);
				}
				else if (is_redirection(commands))
				{
					printf("execute redirection\n");
					execute_redirection(commands, env_list);
				}
				else
				{
					printf("execute command\n");
					execute_commands(commands, env_list);
				}
				free_commands(commands);
			}
		}
		free_token_list(token_list);
		free(input);
	}
	//free(pwd);
	quit_minislay(input, commands, token_list, env_list);
	return (0);
}
