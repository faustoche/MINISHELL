/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:36:11 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/06 12:42:59 by fcrocq           ###   ########.fr       */
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

int handle_word(t_lexer *lexer, int start)
{
	int end;
	int type;
	char *word;
	char *merged_word = NULL;
	int in_quotes = 0;
	char quote_char = 0;
	
	end = start;
	while (lexer->input[end])
	{
		if (!in_quotes)
		{
			if (is_space(lexer->input[end]) || is_separator(lexer->input[end]))
				break;
			if (lexer->input[end] == '\'' || lexer->input[end] == '"')
			{
				quote_char = lexer->input[end];
				in_quotes = 1;
				end++;
				continue ;
			}
			if (!merged_word)
				merged_word = ft_strndup(lexer->input + start, 1);
			else
			{
				char tmp[2] = {lexer->input[end], '\0'};
				char *new_word = ft_strjoin(merged_word, tmp);
				free(merged_word);
				merged_word = new_word;
			}
		}
		else
		{
			if (lexer->input[end] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
				end++;
				continue;
			}
			if (!merged_word)
				merged_word = ft_strndup(lexer->input + end, 1);
			else
			{
				char tmp[2] = {lexer->input[end], '\0'};
				char *new_word = ft_strjoin(merged_word, tmp);
				free(merged_word);
				merged_word = new_word;
			}
		}
		end++;
	}
	if (in_quotes)
	{
		printf(ERR_SYNTAX);
		free(merged_word);
		return (-1);
	}
	if (end == start)
	{
		free(merged_word);
		return (end);
	}
	if (!merged_word)
		word = ft_strndup(lexer->input + start, end - start);
	else
		word = merged_word;
	if (lexer->input[end] == '\'' || lexer->input[end] == '"')
	{
		if (lexer->command)
		{
			printf(ERR_CMD, word);
			free(word);
			return (-1);
		}
	}
	type = get_token_type(word, &lexer->command);
	add_token(lexer, word, ft_strlen(word), type);
	free(word);
	return (end);
}
