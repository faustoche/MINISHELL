/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:36:11 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/28 09:46:19 by fcrocq           ###   ########.fr       */
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

/* Handle standard words (commands, arguments) */

int	handle_word(t_lexer *lexer, int start)
{
	int		end;
	int		type;
	char	*word;

	end = start;
	while (lexer->input[end] && !is_space(lexer->input[end])
		&& !is_separator(lexer->input[end])
		&& lexer->input[end] != '\'' && lexer->input[end] != '"')
		end++;
	if (end == start)
		return (end);
	word = ft_strndup(lexer->input + start, end - start);
	if (!word)
		return (-1);
	type = get_token_type(word, &lexer->command);
	add_token(lexer, word, end - start, type);
	free(word);
	return (end);
}
