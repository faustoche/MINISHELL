/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/11 18:51:39 by fcrocq           ###   ########.fr       */
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

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}	
	return (NULL);
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
			free_env_list(&env_list);
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

t_env	*copy_env_list(t_env *original_env)
{
	t_env	*copy_head;
	t_env	*copy_current;
	t_env	*original_current;
	t_env	*new_element;

	original_current = original_env;
	copy_head = NULL;
	copy_current = NULL;
	while (original_current)
	{
		new_element = malloc(sizeof(t_env));
		if (!new_element)
		{
			free_env_list(&copy_head);
			return (NULL);
		}
		new_element->name = ft_strdup(original_current->name);
		if (!new_element->name)
		{
			free_elements(new_element);
			free_env_list(&copy_head);
			return (NULL);
		}
		new_element->value = ft_strdup(original_current->value);
		if (!new_element->value)
		{
			free_elements(new_element);
			free_env_list(&copy_head);
			return (NULL);
		}
		new_element->next = NULL;
		new_element->cmd = original_current->cmd;
		if (!copy_head)
		{
			copy_head = new_element;
			copy_current = new_element;
		}
		else
		{
			copy_current->next = new_element;
			copy_current = new_element;
		}
		original_current = original_current->next;
	}
	return (copy_head);
}

char	*find_var_value(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
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
