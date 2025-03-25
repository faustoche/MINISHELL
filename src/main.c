/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/25 18:46:01 by faustoche        ###   ########.fr       */
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

int main(int ac, char **av, char **envp)
{
    char    *input;
    t_token *token_list;
    t_cmd   *commands;
    t_env   *env_list;

    (void)ac;
    (void)av;
    env_list = copy_env_list(init_env(envp));
    if (!env_list)
    {
        printf("Error: env variable initialization\n");
        return (-1);
    }
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
				if (has_pipes(commands) == 1)
					execute_pipeline(commands, env_list);
				else if (is_redirection(commands))
					execute_redirection(commands, env_list);
				else
					execute_commands(commands, env_list);
				free_commands(commands);
			}
        }
        free_token_list(token_list);
        free(input);
    }
    free_env_list(env_list);
    clear_history();
    return (0);
}
