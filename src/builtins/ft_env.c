/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:42:02 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/09 13:38:14 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (current->name && current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

void	print_sorted_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (current->name && current->value)
			printf("export %s=\"%s\"\n", current->name, current->value);
		else if (current->name)
			printf("export %s\n", current->name);
		current = current->next;
	}
}
