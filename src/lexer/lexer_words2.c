/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 21:46:17 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 15:43:02 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check syntaxe error and handle specific char and separator */

int	handle_special_char(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'' || c == '"')
	{
		lexer->pos = handle_mixed_quotes(lexer, lexer->pos);
		if (lexer->pos == -1)
			return (-1);
	}
	else if (syntax_error(lexer->input) == -1
		|| input_check(lexer->input) == -1)
		return (-1);
	else if (is_separator(c))
	{
		if (delimiter_error(&lexer->input[lexer->pos]) == -1)
			return (-1);
		lexer->pos = handle_delimiter(lexer, lexer->pos);
	}
	else
		lexer->pos = handle_word(lexer, lexer->pos);
	if (lexer->pos == -1)
		return (-1);
	return (0);
}

/* Ajouter des tokens a la liste chainee */

void	add_token(t_lexer *lexer, char *word, int length, int type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_strndup(word, length);
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	new_token->type = type;
	new_token->next = NULL;
	if (!lexer->tokens)
		lexer->tokens = new_token;
	else
	{
		current = lexer->tokens;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

static char	*append_char_to_word(char *merged_word, char current_char)
{
	char	tmp[2];
	char	*new_word;

	if (!merged_word)
	{
		tmp[0] = current_char;
		tmp[1] = '\0';
		return (ft_strndup(tmp, 1));
	}
	else
	{
		tmp[0] = current_char;
		tmp[1] = '\0';
		new_word = ft_strjoin(merged_word, tmp);
		free(merged_word);
		return (new_word);
	}
}

char	*handle_char(char *merged, char current, int *quotes, char *quote_char)
{
	if (!(*quotes))
	{
		if (current == '\'' || current == '"')
		{
			*quote_char = current;
			*quotes = 1;
			return (merged);
		}
	}
	else if (current == *quote_char)
	{
		*quotes = 0;
		*quote_char = 0;
		return (merged);
	}
	return (append_char_to_word(merged, current));
}
