/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:50:10 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/17 10:16:40 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_merged_token(t_lexer *lexer, char *merged_word, int is_fst_tok)
{
	int	token_type;

	if (is_fst_tok)
		token_type = get_token_type(merged_word, &lexer->command);
	else
		token_type = TOKEN_ARGUMENT;
	add_token(lexer, merged_word, ft_strlen(merged_word), token_type);
	free(merged_word);
	return (-1);
}

static char	*merge_quote_content(char *merged_word, char *quote_content)
{
	char	*temp;
	char	*new_merged;

	temp = merged_word;
	if (merged_word)
		new_merged = ft_strjoin(merged_word, quote_content);
	else
		new_merged = ft_strjoin("", quote_content);
	free(temp);
	return (new_merged);
}

/* Add a non quoted char to the list in progress */

static int	process_non_quote_char(t_lexer *lexer, int end, char **merged_word)
{
	char	temp[2];
	char	*temp_merged;

	temp[0] = lexer->input[end];
	temp[1] = '\0';
	temp_merged = *merged_word;
	if (*merged_word)
		*merged_word = ft_strjoin(*merged_word, temp);
	else
		*merged_word = ft_strjoin("", temp);
	free(temp_merged);
	return (end + 1);
}

static int	process_token_segment(t_lexer *lexer, int start, char **merged_word)
{
	int		end;
	char	quote_type;
	char	*quote_content;

	end = start;
	if (lexer->input[end] == '\'' || lexer->input[end] == '"')
	{
		quote_type = lexer->input[end];
		end++;
		while (lexer->input[end] && lexer->input[end] != quote_type)
			end++;
		if (lexer->input[end] == '\0')
		{
			printf(ERR_SYNTAX);
			return (-1);
		}
		quote_content = ft_strndup(lexer->input + start, end - start + 1);
		*merged_word = merge_quote_content(*merged_word, quote_content);
		free(quote_content);
		return (end + 1);
	}
	return (process_non_quote_char(lexer, start, merged_word));
}

int	handle_mixed_quotes(t_lexer *lexer, int start)
{
	int		end;
	char	*merged_word;

	end = start;
	merged_word = NULL;
	while (lexer->input[end] && !is_space(lexer->input[end]))
	{
		end = process_token_segment(lexer, end, &merged_word);
		if (end == -1)
		{
			free(merged_word);
			return (-1);
		}
	}
	if (merged_word)
		add_merged_token(lexer, merged_word, lexer->command);
	return (end);
}
