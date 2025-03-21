/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/21 12:55:46 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Define token type for each split part of the input according to the macros */

int	get_token_type(char *token, int *command)
{
	if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(token, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(token, ";") == 0)
		return (TOKEN_SEPARATOR);
	if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(token, "(") == 0)
		return (TOKEN_OPEN_PARENT);
	if (ft_strcmp(token, ")") == 0)
		return (TOKEN_CLOSE_PARENT);
	if (ft_strcmp(token, "&&") == 0)
		return (TOKEN_AND);
	if (ft_strcmp(token, "||") == 0)
		return (TOKEN_OR);
	if (*command)
	{
		*command = 0;
		return (TOKEN_COMMAND);
	}
	return (TOKEN_ARGUMENT);
}

int	handle_standard_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env_list)
{
	t_cmd	*new_cmd;

	if (!(*current))
	{
		new_cmd = init_command();
		if (!new_cmd)
		{
			free_commands(*head);
			return (-1);
		}
		new_cmd->env_list = env_list;
		if (!(*head))
			*head = new_cmd;
		else
			(*current)->next = new_cmd;
		*current = new_cmd;
	}
	if (add_args(*token, *current) == -1)
	{
		free_commands(*head);
		return (-1);
	}
	return (0);
}
