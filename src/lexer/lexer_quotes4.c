/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:04:40 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:34:28 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_case(char *input, char *result, int *index, int sq)
{
	if (is_dollar_quote(input, index[0]) && !sq)
	{
		index[0] = handle_dollar_quote(input, result, index[0], &index[1]);
		return (1);
	}
	return (0);
}

int	handle_quote_case(char *input, char *result, int *index, int *quotes)
{
	char	c;

	c = input[index[0]];
	if (c == '\'' || c == '"')
	{
		if (c == '\'' && !quotes[1])
			quotes[0] = handle_quote_state(quotes[0]);
		else if (c == '"' && !quotes[0])
			quotes[1] = handle_quote_state(quotes[1]);
		result[index[1]++] = input[index[0]++];
		return (1);
	}
	return (0);
}

int	handle_edge_quotes(char *input, char *result, int *index)
{
	if ((input[index[0]] == '"' && input[index[0] - 1] == '"'))
	{
		result[index[1]++] = input[index[0]++];
		return (1);
	}
	else if ((input[index[0]] == '\'' && input[index[0] - 1] == '\''))
	{
		result[index[0]++] = input[index[0]++];
		return (1);
	}
	return (0);
}
