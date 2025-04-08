/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:11:11 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 22:14:19 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_escaped_char(char c)
{
	return (c == '"' || c == '\\' || c == '$');
}

int	has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

int is_empty_command(t_cmd *cmd)
{
    return (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0);
}