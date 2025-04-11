/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:53:37 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 18:51:47 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_variable_part7(t_expand *exp)
{
	int		var_start;
	char	*quote_type;

	if (exp->str[exp->i] == '"' || exp->str[exp->i] == '\''
		|| exp->str[exp->i] == '`')
	{
		quote_type = &exp->str[exp->i];
		exp->i++;
		if (exp->str[exp->i] == *quote_type)
		{
			exp->i++;
			return (-1);
		}
		var_start = exp->i;
		while (exp->str[exp->i] && exp->str[exp->i] != *quote_type)
			exp->i++;
		if (exp->str[exp->i])
			exp->i++;
		copy_str_to_result(exp, exp->str + var_start, exp->i - var_start - 1);
		return (-1);
	}
	return (0);
}

static int	process_variable_part8(t_expand *exp, int *var_start, int *var_end)
{
	*var_start = exp->i;
	if (exp->str[exp->i] == '{')
	{
		exp->i++;
		*var_start = exp->i;
		while (exp->str[exp->i] && exp->str[exp->i] != '}')
			exp->i++;
		*var_end = exp->i;
		if (exp->str[exp->i] == '}')
			exp->i++;
	}
	else
	{
		while (exp->str[exp->i] && (isalnum(exp->str[exp->i])
				|| exp->str[exp->i] == '_'))
			exp->i++;
		*var_end = exp->i;
	}
	return (0);
}

static int	process_variable_part9(t_expand *exp, int var_start, int var_end)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_strndup(exp->str + var_start, var_end - var_start);
	if (!var_name)
		return (0);
	var_value = find_var_value(exp->env_list, var_name);
	free(var_name);
	if (var_value)
	{
		if (!copy_str_to_result(exp, var_value, ft_strlen(var_value)))
			return (0);
	}
	return (1);
}

static int	process_variable_begin(t_expand *exp, int *ret)
{
	if (process_variable_part1(exp))
		return (1);
	if (process_variable_part2(exp))
		return (1);
	if (process_variable_part3(exp))
		return (1);
	if (process_variable_part4(exp))
		return (1);
	*ret = process_variable_part5(exp);
	if (*ret)
		return (-1);
	*ret = process_variable_part6(exp);
	if (*ret)
		return (-1);
	return (0);
}

int	process_variable(t_expand *exp)
{
	int	var_start;
	int	var_end;
	int	ret;
	int	result;

	result = process_variable_begin(exp, &ret);
	if (result != 0)
		return (result);
	ret = process_variable_part7(exp);
	if (ret)
		return (-1);
	var_start = 0;
	var_end = 0;
	process_variable_part8(exp, &var_start, &var_end);
	ret = process_variable_part9(exp, var_start, var_end);
	if (ret)
		return (1);
	else
		return (0);
}
