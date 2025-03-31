/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/31 15:19:29 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_args(t_cmd *cmd)
{
	cmd->max_arg = 4;
	cmd->args = malloc(sizeof(char *) * (cmd->max_arg + 1));
	if (!cmd->args)
		return (-1);
	cmd->nb_arg = 0;
	cmd->args[0] = NULL;
	return (0);
}

int	expand_args(t_cmd *cmd)
{
	char	**new_args;
	size_t	i;

	i = 0;
	new_args = malloc(sizeof(char *) * (cmd->max_arg * 2 + 1));
	if (!new_args)
		return (-1);
	while (i <= cmd->nb_arg)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	free(cmd->args);
	cmd->args = new_args;
	cmd->max_arg *= 2;
	return (0);
}

int	add_args(t_token *token, t_cmd *cmd)
{
	int	i;

	if (!cmd->args)
	{
		if (init_args(cmd) == -1)
			return (-1);
	}
	if (cmd->nb_arg >= cmd->max_arg - 1)
	{
		if (expand_args(cmd) == -1)
			return (-1);
	}
	cmd->args[cmd->nb_arg] = ft_strdup(token->value);
	if (!cmd->args[cmd->nb_arg])
	{
		i = -1;
		while (cmd->args[++i])
			free(cmd->args[i]);
		return (-1);
	}
	cmd->nb_arg++;
	cmd->args[cmd->nb_arg] = NULL;
	return (0);
}
