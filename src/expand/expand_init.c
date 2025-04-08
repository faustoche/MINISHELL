/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:01:35 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 17:19:33 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main function to expand variables */

char	*expand_variable(t_env *env_list, char *str, int quote_type)
{
	t_expand	exp;

	if (quote_type == SINGLE_QUOTE)
		return (ft_strdup(str));
	exp.env_list = env_list;
	if (!init_expand_result(str, &exp))
		return (NULL);
	if (!expand_loop(&exp))
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

static int	expand_loop_part2(t_expand *exp)
{
	if (exp->str[exp->i] == '$')
	{
		if (!process_variable(exp))
			return (0);
	}
	else
		exp->result[exp->j++] = exp->str[exp->i++];
	return (1);
}

int	expand_loop(t_expand *exp)
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
			if (!expand_loop_part2(exp))
				return (0);
		}
	}
	exp->result[exp->j] = '\0';
	return (1);
}

// int	expand_loop(t_expand *exp)
// {
// 	while (exp->str[exp->i])
// 	{
// 		if (!check_buffer_size(exp))
// 			return (0);
// 		if (exp->str[exp->i] == '$' && exp->i > 0
// 			&& exp->str[exp->i - 1] == '"' && exp->i + 1 < ft_strlen(exp->str)
// 			&& exp->str[exp->i + 1] == '"')
// 		{
// 			exp->result[exp->j++] = '$';
// 			exp->i += 2;
// 			continue ;
// 		}
// 		if (exp->str[exp->i] == 1 && exp->str[exp->i + 1] == '$')
// 		{
// 			exp->result[exp->j++] = '$';
// 			exp->i += 2;
// 		}
// 		else if (exp->str[exp->i] == '$')
// 		{
// 			if (!process_variable(exp))
// 				return (0);
// 		}
// 		else
// 			exp->result[exp->j++] = exp->str[exp->i++];
// 	}
// 	exp->result[exp->j] = '\0';
// 	return (1);
// }
