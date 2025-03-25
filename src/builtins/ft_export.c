/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:50:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/25 14:09:11 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_var(t_env *env_list, char *name)
{
	t_env	*current;
	
	current = env_list;
	while (current)
	{
		if (strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_env_var(t_env **env_list, char *name, char *value)
{
	t_env	*new_var;
	
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return;
	new_var->name = ft_strdup(name);
	new_var->value = ft_strdup(value);
	new_var->next = *env_list;
	*env_list = new_var;
}


void export_variable(t_env **env_list, char *arg)
{
	char    *name;
	char    *value;
	t_env   *env_var;
	char    *equal_pos;

	if (!arg || *arg == '\0' || *arg == '=')
	{
		printf("errorr: not a valid identifier\n");
		return ;
	}
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		if (equal_pos == arg)
		{
			printf("errror: not a valid identifier\n");
			return ;
		}
		name = ft_strndup(arg, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = ft_strdup("");
	}
	if (!name || !*name)
	{
		printf("error: not a valid identifier\n");
		free(name);
		free(value);
		return ;
	}
	env_var = find_env_var(*env_list, name);
	if (env_var)
	{
		free(env_var->value);
		env_var->value = ft_strdup(value);
	}
	else
		add_env_var(env_list, name, value);
	free(name);
	free(value);
}
