/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:30:49 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/13 20:03:28 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_variable(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_var)
{
	char	*env_str;

	env_str = malloc(ft_strlen(env_var->name) + ft_strlen(env_var->value) + 2);
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, env_var->name);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, env_var->value);
	return (env_str);
}

static int	fill_env_array(char **result, t_env *env_list)
{
	int		count;
	t_env	*cur;
	char	*env_str;

	cur = env_list;
	count = 0;
	while (cur)
	{
		env_str = create_env_string(cur);
		if (!env_str)
		{
			while (--count >= 0)
				free(result[count]);
			return (-1);
		}
		result[count++] = env_str;
		cur = cur->next;
	}
	result[count] = NULL;
	return (0);
}

char	**env_list_to_array(t_env *env_list)
{
	int		count;
	char	**result;

	count = count_env_variable(env_list);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	if (fill_env_array(result, env_list) == -1)
	{
		free(result);
		return (NULL);
	}
	return (result);
}
