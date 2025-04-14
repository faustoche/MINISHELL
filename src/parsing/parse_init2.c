/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:42:08 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:42:31 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_quote_var(int *i, int *j, int *single, int *doubles)
{
	*i = 0;
	*j = 0;
	*single = 0;
	*doubles = 0;
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*res;
	int		in_single;
	int		in_double;

	init_quote_var(&i, &j, &in_single, &in_double);
	if (!str)
		return (NULL);
	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}
