/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/15 18:31:55 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_element(char *env)
{
	t_env	*element;
	char	*pos_equal;

	if (!env)
		return (NULL);
	element = malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	pos_equal = ft_strchr(env, '=');
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
	if (!element->name || !element->value)
		free_elements(element);
	element->next = NULL;
	element->cmd = NULL;
	return (element);
}

static t_env	*create_element_from_envp(char *env_str)
{
	t_env	*new_element;

	new_element = create_env_element(env_str);
	if (!new_element)
		return (NULL);
	return (new_element);
}

static void	add_element_to_list(t_env **env_list, t_env **cur, t_env *new_el)
{
	if (!*env_list)
	{
		*env_list = new_el;
		*cur = new_el;
	}
	else
	{
		(*cur)->next = new_el;
		*cur = new_el;
	}
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
		new_element = create_element_from_envp(envp[i]);
		if (!new_element)
		{
			free_env_list(&env_list);
			return (NULL);
		}
		add_element_to_list(&env_list, &current, new_element);
		i++;
	}
	return (env_list);
}

t_env	*change_var_value(t_env *env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*head;
	char	*new_value;

	head = env_list;
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			new_value = ft_strdup(value);
			if (!new_value)
				return (head);
			free(current->value);
			current->value = new_value;
			break ;
		}
		current = current->next;
	}
	return (head);
}
