/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:01:35 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 10:10:44 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quoted(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (!str)
		return (0);
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
		return (1);
	return (0);
}

/* Main function to expand variables */

char	*expand_variable(t_env *env_list, char *str, int *code)
{
	t_expand	exp;

	if (is_quoted(str))
	{
		return (ft_strdup(str));
	}
	exp.env_list = env_list;
	if (!init_expand_result(str, &exp))
		return (NULL);
	if (!expand_loop(&exp, code))
		return (NULL);
	return (exp.result);
}

char	*init_expand_result(const char *str, t_expand *exp)
{
	size_t	i;

	i = 0;
	exp->capacity = ft_strlen(str) + 1;
	exp->i = 0;
	exp->j = 0;
	exp->str = (char *)str;
	exp->result = malloc(exp->capacity);
	if (!exp->result)
		return (NULL);
	while (i < exp->capacity)
	{
		exp->result[i] = '\0';
		i++;
	}	
	return (exp->result);
}

/* Main expansion loop */

static int	expand_loop_part2(t_expand *exp, int *code)
{
	if (exp->str[exp->i] == '$')
	{
		if (!process_variable(exp, code))
			return (0);
	}
	else
		exp->result[exp->j++] = exp->str[exp->i++];
	return (1);
}

int	expand_loop(t_expand *exp, int *code)
{
	while (exp->str[exp->i])
	{
		if (!check_buffer_size(exp))
			return (0);
		if (exp->str[exp->i] == '$' && exp->i > 0
			&& exp->str[exp->i - 1] == '"' && exp->i + 1 < ft_strlen(exp->str)
			&& exp->str[exp->i + 1] == '"')
		{
			exp->result[exp->j++] = '$';
			exp->i += 2;
			continue ;
		}
		if (exp->str[exp->i] == 1 && exp->str[exp->i + 1] == '$')
		{
			exp->result[exp->j++] = '$';
			exp->i += 2;
		}
		else
		{
			if (!expand_loop_part2(exp, code))
				return (0);
		}
	}
	exp->result[exp->j] = '\0';
	return (1);
}
