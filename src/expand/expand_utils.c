/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:52:10 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/03 12:33:02 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Extracts the name of the variable */

char	*extract_variable_name(t_expand *exp, size_t *len)
{
	size_t	start;

	start = exp->i;
	while (exp->str[exp->i] && (isalnum(exp->str[exp->i])
			|| exp->str[exp->i] == '_'))
		(exp->i)++;
	*len = exp->i - start;
	return (ft_strndup(exp->str + start, *len));
}

/* Copies the variable value into the result buffer */

int	copy_variable_value(t_expand *exp, char *value, char *name)
{
	size_t	value_len;

	if (!value)
	{
		free(name);
		return (-1);
	}
	value_len = ft_strlen(value);
	while (exp->j + value_len + 1 >= exp->capacity)
	{
		if (!resize_result_buffer(exp))
		{
			free(name);
			return (0);
		}
	}
	ft_strcpy(exp->result + exp->j, value);
	exp->j += value_len;
	free(name);
	return (1);
}

/* Processes a variable for expansion */

int	process_variable(t_expand *exp)
{
	size_t	len;
	char	*name;
	char	*value;

	(exp->i)++;
	name = extract_variable_name(exp, &len);
	if (!name)
	{
		free(exp->result);
		exp->result = NULL;
		return (0);
	}
	value = get_env_value(exp->env_list, name);
	return (copy_variable_value(exp, value, name));
}

/* Resizes the result buffer if necessary */

int	resize_result_buffer(t_expand *exp)
{
	char	*temp;

	exp->capacity *= 2;
	// exp->result = NULL; soit j'ai un conditional jump, soit ca ne fonctio
	temp = ft_realloc(exp->result, exp->capacity);
	if (!temp)
	{
		free(exp->result);
		exp->result = NULL;
		return (0);
	}
	exp->result = temp;
	return (1);
}

/* Checks and resizes buffer if necessary */

int	check_buffer_size(t_expand *exp)
{
	if (exp->j + 1 >= exp->capacity)
	{
		if (!resize_result_buffer(exp))
			return (0);
	}
	return (1);
}
