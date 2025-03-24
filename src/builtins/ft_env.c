/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:42:02 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/24 20:29:52 by faustoche        ###   ########.fr       */
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
	printf("ft_env : %p\n", cmd->env_list);
}
