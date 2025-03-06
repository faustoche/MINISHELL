/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:28:18 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/06 09:01:23 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env_list(t_env *env_list)
{
	t_env *tmp;
    
    while (env_list)
    {
		tmp = env_list;
        env_list = env_list->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}

void	free_elements(t_env *element)
{
	if (element)
	{
		free(element->name);
		free(element->value);
		free(element);
	}
}

