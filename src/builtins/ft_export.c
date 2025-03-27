/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:50:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/27 11:10:08 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_env	*ft_export(t_env *env_list, char *arg)
{
	t_env	*new_env_list;
	t_env	*new_var;
	t_env	*current;
	char	*name;
	char	*value;
	char	*equal_pos;

	new_env_list = copy_env_list(env_list);
	if (!arg || *arg == '\0' || *arg == '=')
	{
		printf(ERR_SYNTAX);
		free_env_list(env_list);
		free_env_list(new_env_list);
		return (env_list);
	}
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		if (equal_pos == arg)
		{
			printf(ERR_SYNTAX);
			free_env_list(new_env_list);
			return (env_list);
		}
		name = ft_strndup(arg, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (!name || !*name || !value || !*value)
	{
		free(name);
		free(value);
		free_env_list(new_env_list);
		return (env_list);
	}
	current = new_env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			break ;
		}
		current = current->next;
	}
	if (!current)
	{
		new_var = malloc(sizeof(t_env));
		if (!new_var)
			return (NULL);
		new_var->name = ft_strdup(name);
		new_var->value = ft_strdup(value);
		ft_lstadd_back(&new_env_list, new_var);
	}
	free_env_list(env_list);
	free(name);
	free(value);
	return (new_env_list);
}
