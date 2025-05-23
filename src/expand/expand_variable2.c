/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:54:53 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 11:29:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_dq_dollar(t_expand *exp)
{
	if (exp->i > 0 && exp->str[exp->i - 1] == '"'
		&& exp->str[exp->i] == '$' && exp->str[exp->i + 1] == '"')
	{
		exp->result[exp->j++] = '$';
		exp->i++;
		if (exp->str[exp->i] == '"')
			exp->i++;
		return (1);
	}
	return (0);
}

int	expand_dq_check(t_expand *exp)
{
	int	temp_i;

	if (exp->i > 0 && exp->str[exp->i - 1] == '"' && exp->str[exp->i] == '$')
	{
		temp_i = exp->i + 1;
		while (exp->str[temp_i] && exp->str[temp_i] != ' '
			&& exp->str[temp_i] != '"' && exp->str[temp_i] != '\'')
			temp_i++;
		if (exp->str[temp_i] == '"' && exp->str[temp_i + 1]
			&& !is_space(exp->str[temp_i + 1]) && exp->str[temp_i + 1] != '\0')
		{
			exp->result[exp->j++] = exp->str[exp->i - 1];
			exp->result[exp->j++] = exp->str[exp->i++];
			return (1);
		}
	}
	return (0);
}

int	expand_quote_var(t_expand *exp)
{
	char	quote;
	size_t	start;

	if (exp->str[exp->i] == '$' && exp->i + 1 < ft_strlen(exp->str)
		&& (exp->str[exp->i + 1] == '"' || exp->str[exp->i + 1] == '\''))
	{
		quote = exp->str[exp->i + 1];
		exp->i += 2;
		start = exp->i;
		while (exp->str[exp->i] && exp->str[exp->i] != quote)
			exp->i++;
		if (start < exp->i)
			copy_str_to_result(exp, exp->str + start, exp->i - start);
		if (exp->str[exp->i] == quote)
			exp->i++;
		return (1);
	}
	return (0);
}

int	expand_one_dollar(t_expand *exp)
{
	if (!exp->str[exp->i + 1])
	{
		exp->result[exp->j++] = '$';
		exp->i++;
		return (1);
	}
	return (0);
}
