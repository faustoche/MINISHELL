/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:58:17 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/14 07:57:10 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_env	*create_new_element(void)
{
	t_env	*new_element;

	new_element = malloc(sizeof(t_env));
	if (!new_element)
		return (NULL);
	new_element->name = NULL;
	new_element->value = NULL;
	new_element->next = NULL;
	new_element->cmd = NULL;
	return (new_element);
}

static int	copy_element_data(t_env *new_element, t_env *original_element)
{
	new_element->name = ft_strdup(original_element->name);
	if (!new_element->name)
		return (0);
	new_element->value = ft_strdup(original_element->value);
	if (!new_element->value)
		return (0);
	new_element->cmd = original_element->cmd;
	return (1);
}

static void	copy_list(t_env **head, t_env **current, t_env *new_el, int fst_el)
{
	if (fst_el)
	{
		*head = new_el;
		*current = new_el;
	}
	else
	{
		(*current)->next = new_el;
		*current = new_el;
	}
}

t_env	*copy_env_list(t_env *original_env)
{
	t_env	*copy_head;
	t_env	*copy_current;
	t_env	*original_current;
	t_env	*new_element;

	copy_head = NULL;
	copy_current = NULL;
	original_current = original_env;
	while (original_current)
	{
		new_element = create_new_element();
		if (!new_element)
		{
			free_env_list(&copy_head);
			return (NULL);
		}
		if (!copy_element_data(new_element, original_current))
		{
			(free_elements(new_element), free_env_list(&copy_head));
			return (NULL);
		}
		copy_list(&copy_head, &copy_current, new_element, !copy_head);
		original_current = original_current->next;
	}
	return (copy_head);
}
