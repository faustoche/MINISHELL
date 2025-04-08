/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:02:49 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 19:07:49 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_quote(char *input, int i)
{
	return (input[i] == '$' && (input[i + 1] == '"' || input[i + 1] == '\''));
}

int	handle_dollar_quote(char *input, char *result, int i, int *j)
{
	char	quote;

	quote = input[i + 1];
	i += 2;
	while (input[i] && input[i] != quote)
		result[(*j)++] = input[i++];
	if (input[i] == quote)
		i++;
	return (i);
}

int	handle_quote_state(int state)
{
	if (state == 0)
		return (1);
	else
		return (0);
}

int	handle_dollar_dq(char *input, char *result, int i, int *j)
{
	result[(*j)++] = '$';
	i++;
	if (input[i] == '"')
		i++;
	return (i);
}

void	handle_outside_quotes(char *input, char *processed, t_state *state)
{
	if (input[state->i + 1] == '$')
	{
		state->i++;
		processed[state->j++] = 1;
		processed[state->j++] = input[state->i++];
	}
	else
	{
		state->i++;
		processed[state->j++] = input[state->i++];
	}
}
