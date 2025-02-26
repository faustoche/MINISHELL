/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:44:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_args(t_cmd *command)
{
	command->max_arg = 4;
	command->args = malloc(sizeof(char *) * (command->max_arg + 1));
	if (!command->args)
		return (-1);
	command->nb_arg = 0;
	command->args[0] = NULL;
	return (0);
}

/* Expand the number of args according to the input */

int	expand_args(t_cmd *command)
{
	char	**new_args;
	size_t	i;

	i = 0;
	new_args = malloc(sizeof(char *) * (command->max_arg * 2 + 1));
	if (!new_args)
		return (-1);
	while (i <= command->nb_arg)
	{
		new_args[i] = command->args[i];
		i++;
	}
	free(command->args);
	command->args = new_args;
	command->max_arg *= 2;
	return (0);
}

/* Attention : un leak dans cette fonction */

int	add_args(t_cmd *cmd, char *arg)
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
		cmd->args[cmd->nb_arg] = ft_strdup(arg);
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
