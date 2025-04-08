/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:36:11 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 18:07:00 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_word(t_lexer *lexer, char *word, int end)
{
	int	type;

	type = get_token_type(word, &lexer->command);
	add_token(lexer, word, ft_strlen(word), type);
	free(word);
	return (end);
}

static int	preliminary_errors(char *word, int start, int end, int in_quotes)
{
	if (in_quotes)
	{
		printf(ERR_SYNTAX);
		free(word);
		return (-1);
	}
	if (end == start)
	{
		free(word);
		return (end);
	}
	return (0);
}

static int	final_word(t_lexer *lexer, char *merged_word, int start, int end)
{
	char	*word;

	word = create_final_word(lexer, merged_word, start, end);
	if (check_quote_errors(lexer, word, end) < 0)
		return (-1);
	return (process_word(lexer, word, end));
}

static int	parse_word(t_lexer *lexer, int start, char **word, int *in_quotes)
{
	int		end;
	char	quote_char;

	end = start;
	quote_char = 0;
	*in_quotes = 0;
	*word = NULL;
	while (lexer->input[end])
	{
		if (!(*in_quotes) && (is_space(lexer->input[end])
				|| is_separator(lexer->input[end])))
			break ;
		*word = handle_char(*word, lexer->input[end], in_quotes, &quote_char);
		if ((*in_quotes && lexer->input[end] == quote_char)
			|| (!(*in_quotes) && (lexer->input[end] == '\''
					|| lexer->input[end] == '"')))
		{
			end++;
			continue ;
		}
		end++;
	}
	return (end);
}

int	handle_word(t_lexer *lexer, int start)
{
	int		end;
	int		in_quotes;
	int		result;
	char	*merged_word;

	end = parse_word(lexer, start, &merged_word, &in_quotes);
	result = preliminary_errors(merged_word, start, end, in_quotes);
	if (result != 0)
		return (result);
	return (final_word(lexer, merged_word, start, end));
}

/* Handle standard words (commands, arguments) */

// int handle_word(t_lexer *lexer, int start)
// {
// 	int end;
// 	int type;
// 	char *word;
// 	char *merged_word = NULL;
// 	int in_quotes = 0;
// 	char quote_char = 0;

// 	end = start;
// 	while (lexer->input[end])
// 	{
// 		if (!in_quotes)
// 		{
// 			if (is_space(lexer->input[end]) || is_separator(lexer->input[end]))
// 				break;
// 			if (lexer->input[end] == '\'' || lexer->input[end] == '"')
// 			{
// 				quote_char = lexer->input[end];
// 				in_quotes = 1;
// 				end++;
// 				continue ;
// 			}
// 			if (!merged_word)
// 				merged_word = ft_strndup(lexer->input + start, 1);
// 			else
// 			{
// 				char tmp[2] = {lexer->input[end], '\0'};
// 				char *new_word = ft_strjoin(merged_word, tmp);
// 				free(merged_word);
// 				merged_word = new_word;
// 			}
// 		}
// 		else
// 		{
// 			if (lexer->input[end] == quote_char)
// 			{
// 				in_quotes = 0;
// 				quote_char = 0;
// 				end++;
// 				continue;
// 			}
// 			if (!merged_word)
// 				merged_word = ft_strndup(lexer->input + end, 1);
// 			else
// 			{
// 				char tmp[2] = {lexer->input[end], '\0'};
// 				char *new_word = ft_strjoin(merged_word, tmp);
// 				free(merged_word);
// 				merged_word = new_word;
// 			}
// 		}
// 		end++;
// 	}
// 	if (in_quotes)
// 	{
// 		printf(ERR_SYNTAX);
// 		free(merged_word);
// 		return (-1);
// 	}
// 	if (end == start)
// 	{
// 		free(merged_word);
// 		return (end);
// 	}
// 	if (!merged_word)
// 		word = ft_strndup(lexer->input + start, end - start);
// 	else
// 		word = merged_word;
// 	if (lexer->input[end] == '\'' || lexer->input[end] == '"')
// 	{
// 		if (lexer->command)
// 		{
// 			printf(ERR_CMD, word);
// 			free(word);
// 			return (-1);
// 		}
// 	}
// 	type = get_token_type(word, &lexer->command);
// 	add_token(lexer, word, ft_strlen(word), type);
// 	free(word);
// 	return (end);
// }
