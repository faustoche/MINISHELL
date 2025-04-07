/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:14:00 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/07 12:11:51 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quit_minislay(char *line, t_cmd *cmd, t_token *token, t_env *env)
{
	t_token	*tmp;

	free(line);
	if (cmd)
		free_commands(cmd);
	while (token)
	{
		tmp = token->next;
		free(token);
		token = tmp;
	}
	free_env_list(&env);
	clear_history();
}

void	clean_exit(t_token *tokens, char *input, t_cmd *commands)
{
	if (tokens)
		free_token_list(tokens);
	if (input)
		free(input);
	if (commands)
		free_commands(commands);
}

int	print_error_message(char *str)
{
	printf("%s\n", str);
	return (EXIT_FAILURE);
}
