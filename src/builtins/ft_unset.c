/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:04:05 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/13 20:49:11 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*unset_utils(t_env *env_list, t_env *new_env_list, char *name)
{
	if (name == NULL || *name == '\0')
	{
		free_env_list(&env_list);
		return (new_env_list);
	}
	if (!is_valid_identifier(name))
	{
		free_env_list(&env_list);
		return (new_env_list);
	}
	return (NULL);
}

static void	remove_env_node(t_env **env_list, t_env *prev, t_env *target)
{
	if (prev)
		prev->next = target->next;
	else
		*env_list = target->next;
	free(target->name);
	free(target->value);
	free(target);
}

t_env	*ft_unset(t_env *env_list, char *name)
{
	t_env	*new_env_list;
	t_env	*current;
	t_env	*prev;
	t_env	*result;

	if (name == NULL || *name == '\0')
		return (env_list);
	new_env_list = copy_env_list(env_list);
	current = new_env_list;
	prev = NULL;
	while (current)
	{
		result = unset_utils(env_list, new_env_list, name);
		if (result != NULL)
			return (result);
		if (strcmp(current->name, name) == 0)
		{
			remove_env_node(&new_env_list, prev, current);
			break ;
		}
		prev = current;
		current = current->next;
	}
	free_env_list(&env_list);
	return (new_env_list);
}

// t_env *ft_unset(t_env *env_list, char *name)
// {
// 	t_env	*new_env_list;
// 	t_env	*current;
// 	t_env	*result;
// 	t_env	*prev;

// 	if (name == NULL || *name == '\0')
// 		return (env_list);
// 	new_env_list = copy_env_list(env_list);
// 	current = new_env_list;
// 	prev = NULL;
// 	while (current)
// 	{
// 		result = unset_utils(env_list, new_env_list, name);
// 		if (result != NULL)
// 			return (result);
// 		if (strcmp(current->name, name) == 0)
// 		{
// 			if (prev)
// 				prev->next = current->next;
// 			else
// 				new_env_list = current->next;
// 			free(current->name);
// 			free(current->value);
// 			free(current);
// 			break ;
// 		}
// 		prev = current;
// 		current = current->next;
// 	}
// 	free_env_list(&env_list);
// 	return (new_env_list);
// }
