/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:44:30 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;

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
		commands = parse_commands(token_list);
		if (commands)
		{
			print_commands(commands); // execute commande a la place ici
			free_commands(commands);
		}
		free_token_list(token_list);
		free(input);
	}
	clear_history();
	return (0);
}

/* Affiche le prompt et récupère l'entrée utilisateur */

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
