/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:52:10 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 17:54:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Extracts the name of the variable */

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

/* Resizes the result buffer if necessary */

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

// int	process_variable(t_expand *exp)
// {
// 	int		var_start;
// 	int		var_end;
// 	char	*var_name;
// 	char	*var_value;
// 	char	*quote_type;
// 	char	quote;
// 	size_t	start;
// 	int		temp_i;

// 	if (exp->i > 0 && exp->str[exp->i - 1] == '"'
// 		&& exp->str[exp->i] == '$' && exp->str[exp->i + 1] == '"')
// 	{
// 		exp->result[exp->j++] = '$';
// 		exp->i++;
// 		if (exp->str[exp->i] == '"')
// 			exp->i++;
// 		return (1);
// 	}
// 	if (exp->i > 0 && exp->str[exp->i - 1] == '"' && exp->str[exp->i] == '$')
// 	{
// 		temp_i = exp->i + 1;
// 		while (exp->str[temp_i] && exp->str[temp_i] != ' '
// 			&& exp->str[temp_i] != '"' && exp->str[temp_i] != '\'')
// 			temp_i++;
// 		if (exp->str[temp_i] == '"' && exp->str[temp_i + 1]
// 			&& !is_space(exp->str[temp_i + 1]) && exp->str[temp_i + 1] != '\0')
// 		{
// 			exp->result[exp->j++] = exp->str[exp->i - 1];
// 			exp->result[exp->j++] = exp->str[exp->i++];
// 			return (1);
// 		}
// 	}
// 	if (exp->str[exp->i] == '$' && exp->i + 1 < ft_strlen(exp->str)
// 		&& (exp->str[exp->i + 1] == '"' || exp->str[exp->i + 1] == '\''))
// 	{
// 		quote = exp->str[exp->i + 1];
// 		exp->i += 2;
// 		start = exp->i;
// 		while (exp->str[exp->i] && exp->str[exp->i] != quote)
// 			exp->i++;
// 		if (start < exp->i)
// 			copy_str_to_result(exp, exp->str + start, exp->i - start);
// 		if (exp->str[exp->i] == quote)
// 			exp->i++;
// 		return (1);
// 	}
// 	if (!exp->str[exp->i + 1])
// 	{
// 		exp->result[exp->j++] = '$';
// 		exp->i++;
// 		return (-1);
// 	}
// 	exp->i++;
// 	if (exp->str[exp->i] != '{' && !isalnum(exp->str[exp->i])
// 		&& exp->str[exp->i] != '_'
// 		&& exp->str[exp->i] != '"' && exp->str[exp->i] != '\''
// 		&& exp->str[exp->i] != '`')
// 	{
// 		exp->result[exp->j++] = '$';
// 		return (-1);
// 	}
// 	if (isdigit(exp->str[exp->i]))
// 	{
// 		exp->i++;
// 		return (-1);
// 	}
// 	if (exp->str[exp->i] == '"' || exp->str[exp->i] == '\''
// 		|| exp->str[exp->i] == '`')
// 	{
// 		quote_type = &exp->str[exp->i];
// 		exp->i++;
// 		if (exp->str[exp->i] == *quote_type)
// 		{
// 			exp->i++;
// 			return (-1);
// 		}
// 		var_start = exp->i;
// 		while (exp->str[exp->i] && exp->str[exp->i] != *quote_type)
// 			exp->i++;
// 		if (exp->str[exp->i])
// 			exp->i++;
// 		copy_str_to_result(exp, exp->str + var_start, exp->i - var_start - 1);
// 		return (-1);
// 	}
// 	var_start = exp->i;
// 	if (exp->str[exp->i] == '{')
// 	{
// 		exp->i++;
// 		var_start = exp->i;
// 		while (exp->str[exp->i] && exp->str[exp->i] != '}')
// 			exp->i++;
// 		var_end = exp->i;
// 		if (exp->str[exp->i] == '}')
// 			exp->i++;
// 	}
// 	else
// 	{
// 		while (exp->str[exp->i] && (isalnum(exp->str[exp->i])
// 				|| exp->str[exp->i] == '_'))
// 			exp->i++;
// 		var_end = exp->i;
// 	}
// 	var_name = ft_strndup(exp->str + var_start, var_end - var_start);
// 	if (!var_name)
// 		return (0);
// 	var_value = get_env_value(exp->env_list, var_name);
// 	free(var_name);
// 	if (var_value)
// 	{
// 		if (!copy_str_to_result(exp, var_value, ft_strlen(var_value)))
// 			return (0);
// 	}
// 	return (1);
// }
