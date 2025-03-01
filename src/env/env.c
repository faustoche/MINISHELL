/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/01 16:19:53 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Créer un nouvel élément d'environnement */

t_env	*create_env_element(char *env)
{
	t_env	*element;
	char	*pos_equal;

	if (!env)
		return (NULL);
	element = malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	pos_equal = ft_strchr(env, "=");
	if (!pos_equal)
	{
		element->name = ft_strdup(env);
		element->value = ft_strdup("");
	}
	else
	{
		element->name = ft_strndup(env, pos_equal - env);
		element->value = ft_strdup(pos_equal + 1);
	}
	free_elements(element);
	element->next = NULL;
	return (element);
}

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

int	free_elements(t_env *element)
{
	if (!element->name || !element->name)
	{
		if (element->name)
			free(element->name);
		if (element->value)
			free(element->value);
		free(element);
		return (NULL);
	}
}
