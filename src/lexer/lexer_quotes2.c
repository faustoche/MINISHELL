/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:25:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 11:49:39 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialize_vars(int *index, int *sq, int *dq)
{
	index[0] = 0;
	index[1] = 0;
	*sq = 0;
	*dq = 0;
}

static int	handle_dollar_in_dq(char *input, char *result, int *index, int dq)
{
	if (input[index[0]] == '$' && dq
		&& input[index[0] + 1] == '"' && input[index[0] - 1] == '"')
	{
		index[0] = handle_dollar_dq(input, result, index[0], &index[1]);
		return (1);
	}
	return (0);
}

static void	process_char(char *input, char *result, int *index, int *quotes)
{
	if (handle_dollar_case(input, result, index, quotes[0]))
		return ;
	if (handle_quote_case(input, result, index, quotes))
		return ;
	if (handle_edge_quotes(input, result, index))
	{
		return ;
	}
	if (handle_dollar_in_dq(input, result, index, quotes[1]))
		return ;
	result[index[1]++] = input[index[0]++];
}

static char	*process_input(char *input, char *result)
{
	int	index[2];
	int	quotes[2];

	initialize_vars(index, &quotes[0], &quotes[1]);
	while (input[index[0]])
		process_char(input, result, index, quotes);
	result[index[1]] = '\0';
	return (result);
}

char	*fix_dollar_quote(char *input)
{
	char	*dup;
	char	*result;

	result = malloc(strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	result = process_input(input, result);
	if (strlen(result) == 0 || ft_strcmp(result, input) == 0)
	{
		dup = ft_strdup(input);
		free(result);
		return (dup);
	}
	return (result);
}
