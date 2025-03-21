/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:42:02 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/21 17:00:40 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_env(t_cmd *cmd)
{
	t_env	*current;

	current = cmd->env_list;
    while (current)
    {
        printf("%s=%s\n", current->name, current->value);
		current = current->next;
    }
}
