/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/03 20:49:02 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;
	t_env	*env_list;
	
	(void)ac;
	(void)av;
	env_list = init_env(envp);
	if (!env_list)
	{
		printf("Erreur d'initialisation des varaibles d'environnement\n");
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
		commands = parse_commands(token_list);
		if (commands)
		{
			print_commands(commands); // execute commande a la place ici
			free_commands(commands);
		}
		free_token_list(token_list);
		free(input);
	}
	free_env_list(env_list);
	clear_history();
	return (0);
}

char	*prompt(void)
{
	char	*line;

	line = readline("minislay> \033[1;32m→\033[0m \033[1;34m~\033[0m ");
	if (line && *line)
		add_history(line);
	return (line);
}

void	print_welcome_message(void)
{
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▖   ▗▖ \n");
	printf("\033[1;36m▐▛▚▞▜▌  █  ▐▛▚▖▐▌  █  ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌\n");
	printf("\033[1;36m▐▌  ▐▌  █  ▐▌ ▝▜▌  █   ▝▀▚▖▐▛▀▜▌▐▛▀▀▘▐▌   ▐▌ \n");
	printf("\033[1;36m▐▌  ▐▌▗▄█▄▖▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖▐▙▄▄▖\n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m              BY       \n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m            GHIEONG    \n");
	printf("\033[1;36m              AND      \n");
	printf("\033[1;36m            FCROCQ     \n");
	printf("\033[1;36m       ******************\n");
	printf("\033[1;36m       ******************\n\033[0m");
	printf("\n");
	printf("\n");
}
