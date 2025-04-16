/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:50:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 08:42:31 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	extract_name_value(char *arg, char **name, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*name = ft_strndup(arg, equal_pos - arg);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
		return (0);
	}
	if (!is_valid_identifier(*name))
	{
		printf(ERR_EXP, *name);
		free(*name);
		if (*value)
			free(*value);
		*name = NULL;
		*value = NULL;
		return (0);
	}
	return (1);
}

static int	modify_env_var(t_env *env_list, char *name, char *value)
{
	t_env	*current;

	if (!name || !*name)
		return (0);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = ft_strdup("");
			return (1);
		}
		current = current->next;
	}
	return (0);
}

static t_env	*create_env_var(t_env *env_list, char *name, char *value)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->name = ft_strdup(name);
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = ft_strdup("");
	new_var->next = NULL;
	ft_lstadd_back(&env_list, new_var);
	return (env_list);
}

static t_env	*update_env_var(t_env *env_list, char *name, char *value)
{
	t_env	*result;

	if (!name || !*name)
		return (env_list);
	if (modify_env_var(env_list, name, value))
		return (env_list);
	result = create_env_var(env_list, name, value);
	if (!result)
		return (NULL);
	return (result);
}

t_env	*ft_export(t_env *env_list, char *arg, int *code)
{
	t_env	*new_env_list;
	char	*name;
	char	*value;

	new_env_list = copy_env_list(env_list);
	if (!arg || *arg == '\0' || arg[0] == '$')
	{
		free_env_list(&env_list);
		return (no_args_export(new_env_list));
	}
	if (!extract_name_value(arg, &name, &value))
	{
		if (code)
			*code = 1;
		(free(name), free_env_list(&env_list));
		return (new_env_list);
	}
	new_env_list = update_env_var(new_env_list, name, value);
	if (!new_env_list)
	{
		(free(name), free(value), free_env_list(&env_list));
		return (NULL);
	}
	(free_env_list(&env_list), free(name), free(value));
	return (new_env_list);
}
