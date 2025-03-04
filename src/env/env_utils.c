/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:28:18 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/04 14:58:46 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*current;
	t_env	*new_element;
	int		i;

	env_list = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_element = create_env_element(envp[i]);
		if (!new_element)
		{
			free_env_list(env_list);
			return (NULL);
		}
		if (!env_list)
		{
			env_list = new_element;
			current = env_list;
		}
		else
		{
			current->next = new_element;
			current = new_element;
		}
		i++;
	}
	return (env_list);
}

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

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current = env_list;

	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}	
	return (getenv(name));
}