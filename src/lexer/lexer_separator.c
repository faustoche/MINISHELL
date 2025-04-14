/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_separator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:08:09 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:35:40 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifier si l'index est un delimitateur */

int	handle_delimiter(t_lexer *lexer, int i)
{
	char	*delim;
	int		len;
	int		type;

	len = 1;
	if ((lexer->input[i] == '>' && lexer->input[i + 1] == '>')
		|| (lexer->input[i] == '<' && lexer->input[i + 1] == '<'))
		len = 2;
	delim = ft_strndup(lexer->input + i, len);
	if (!delim)
		return (-1);
	type = get_token_type(delim, &lexer->command);
	add_token(lexer, delim, len, type);
	free(delim);
	if (type == TOKEN_PIPE || type == TOKEN_SEP)
		lexer->command = 1;
	return (i + len);
}

static void	toggle_quote_and_copy(char *input, char *processed, t_state *state)
{
	char	c;

	c = input[state->i];
	if (c == '\'' && !state->doubles)
		state->singles = !state->singles;
	else if (c == '"' && !state->singles)
		state->doubles = !state->doubles;
	processed[state->j++] = input[state->i++];
}

static void	handle_backslash_quotes(char *input, char *proc, t_state *state)
{
	if (state->singles)
		proc[state->j++] = input[state->i++];
	else if (state->doubles)
	{
		if (is_escaped_char(input[state->i + 1]))
		{
			proc[state->j++] = input[state->i++];
			proc[state->j++] = input[state->i++];
		}
		else
			proc[state->j++] = input[state->i++];
	}
}

static void	handle_backslash(char *input, char *processed, t_state *state)
{
	if (state->singles || state->doubles)
		handle_backslash_quotes(input, processed, state);
	else
		handle_outside_quotes(input, processed, state);
}

char	*handle_escape_char(char *input)
{
	char	*processed;
	t_state	state;
	int		len;

	len = ft_strlen(input);
	processed = malloc(sizeof(char) * (len + 1));
	if (!processed)
		return (NULL);
	state.i = 0;
	state.j = 0;
	state.singles = 0;
	state.doubles = 0;
	while (state.i < len)
	{
		if ((input[state.i] == '\'' && !state.doubles)
			|| (input[state.i] == '"' && !state.singles))
			toggle_quote_and_copy(input, processed, &state);
		else if (input[state.i] == '\\' && (state.i + 1) < len)
			handle_backslash(input, processed, &state);
		else
			processed[state.j++] = input[state.i++];
	}
	processed[state.j] = '\0';
	return (processed);
}
