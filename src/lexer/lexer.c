/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:36:11 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/07 10:48:15 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_lexer(t_lexer *lexer, char *input)
{
	lexer->input = input;
	lexer->tokens = NULL;
	lexer->pos = 0;
	lexer->command = 1;
}

/* Analyse de l'entrée de caractère pour créer une liste de tokens */

t_token	*lexing(char *input)
{
	t_lexer	lexer;
	int		result;

	if (!input || !*input)
		return (NULL);
	if (character_error(input) == -1)
		return (NULL);
	init_lexer(&lexer, input);
	while (lexer.input[lexer.pos])
	{
		skip_space(&lexer);
		if (!lexer.input[lexer.pos])
			break ;
		result = handle_special_char(&lexer);
		if (result <= 0)
		{
			free_token_list(lexer.tokens);
			return (NULL);
		}
	}
	return (lexer.tokens);
}

int	handle_special_char(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
		lexer->pos = single_quotes(lexer, lexer->pos);
	else if (c == '"')
		lexer->pos = double_quotes(lexer, lexer->pos);
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
	return (1);
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

int	handle_word(t_lexer *lexer, int start)
{
	int		end;
	char	*word;
	int		type;

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
