/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_separator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:08:09 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/07 22:14:23 by faustoche        ###   ########.fr       */
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
	if (type == TOKEN_PIPE || type == TOKEN_SEPARATOR)
		lexer->command = 1;
	return (i + len);
}

static void	toggle_quote_and_copy(char *input, char *processed, t_state *state)
{
	char	c = input[state->i];

	if (c == '\'' && !state->doubles)
		state->singles = !state->singles;
	else if (c == '"' && !state->singles)
		state->doubles = !state->doubles;
	processed[state->j++] = input[state->i++];
}

static void	handle_backslash_in_quotes(char *input, char *processed, t_state *state)
{
	if (state->singles)
		processed[state->j++] = input[state->i++];
	else if (state->doubles)
	{
		if (is_escaped_char(input[state->i + 1]))
		{
			processed[state->j++] = input[state->i++];
			processed[state->j++] = input[state->i++];
		}
		else
			processed[state->j++] = input[state->i++];
	}
}

static void	handle_backslash_outside_quotes(char *input, char *processed, t_state *state)
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

static void	handle_backslash(char *input, char *processed, t_state *state)
{
	if (state->singles || state->doubles)
		handle_backslash_in_quotes(input, processed, state);
	else
		handle_backslash_outside_quotes(input, processed, state);
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
		if ((input[state.i] == '\'' && !state.doubles) || (input[state.i] == '"' && !state.singles))
			toggle_quote_and_copy(input, processed, &state);
		else if (input[state.i] == '\\' && (state.i + 1) < len)
			handle_backslash(input, processed, &state);
		else
			processed[state.j++] = input[state.i++];
	}
	processed[state.j] = '\0';
	return (processed);
}


// char    *handle_escape_char(char *input)
// {
// 	char	*processed;
// 	int		i;
// 	int		j;
// 	int		len;
// 	int		singles;
// 	int		doubles;

// 	len = ft_strlen(input);
// 	singles = 0;
// 	doubles = 0;
// 	i = 0;
// 	j = 0;
// 	processed = malloc(sizeof(char) * (len + 1));
// 	if (!processed)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		if (input[i] == '\'' && !doubles)
// 		{
// 			if (singles == 0)
// 				singles = 1;
// 			else
// 				singles = 0;
// 			processed[j++] = input[i++];
// 		}
// 		else if (input[i] == '"' && !singles)
// 		{
// 			if (doubles == 0)
// 				doubles = 1;
// 			else
// 				doubles = 0;
// 			processed[j++] = input[i++];
// 		}
// 		else if (input[i] == '\\' && (i + 1) < len)
// 		{
// 			if (singles)
// 				processed[j++] = input[i++];
// 			else if (doubles)
// 			{
// 				if (input[i + 1] == '"' || input[i + 1] == '\\' || input[i + 1] == '$')
// 				{
// 					processed[j++] = input[i++];
// 					processed[j++] = input[i++];
// 				}
// 				else
// 					processed[j++] = input[i++];
// 			}
// 			else
// 			{
// 				if (input[i + 1] == '$')
// 				{
// 					i++;
// 					processed[j++] = 1;
// 					processed[j++] = input[i++];
// 				}
// 				else
// 				{
// 					i++;
// 					processed[j++] = input[i++];
// 				}
// 			}
// 		}
// 		else
// 			processed[j++] = input[i++];
// 	}
// 	processed[j] = '\0';
// 	return (processed);
// }
