/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:52:10 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 10:10:19 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_variable_name(t_expand *exp, size_t *len)
{
	size_t	start;

	start = exp->i;
	if (!exp->str[exp->i] || (!isalpha(exp->str[exp->i])
			&& exp->str[exp->i] != '_'))
	{
		*len = 0;
		return (NULL);
	}
	while (exp->str[exp->i] && (isalnum(exp->str[exp->i])
			|| exp->str[exp->i] == '_'))
		(exp->i)++;
	*len = exp->i - start;
	return (ft_strndup(exp->str + start, *len));
}

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

int	resize_result_buffer(t_expand *exp)
{
	char	*temp;

	exp->capacity *= 2;
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

int	check_buffer_size(t_expand *exp)
{
	if (exp->j + 1 >= exp->capacity)
	{
		if (!resize_result_buffer(exp))
			return (0);
	}
	return (1);
}

int	copy_str_to_result(t_expand *exp, char *str, int len)
{
	char	*new_buff;
	int		i;

	if (exp->j + len >= exp->capacity)
	{
		exp->capacity = exp->capacity * 2 + len;
		new_buff = ft_realloc(exp->result, exp->capacity);
		if (!new_buff)
			return (0);
		exp->result = new_buff;
	}
	i = 0;
	while (i < len)
	{
		exp->result[exp->j++] = str[i];
		i++;
	}
	return (1);
}
