/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:54:08 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 10:10:14 by fcrocq           ###   ########.fr       */
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

static t_token	*tokenize_input(char *input)
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
		if (result == -1)
		{
			free_token_list(lexer.tokens);
			return (NULL);
		}
	}
	return (lexer.tokens);
}

/* Check if the token has any problems */

static t_token	*validate_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if ((current->type == REDIR_IN || current->type == REDIR_OUT
				|| current->type == HEREDOC || current->type == REDIR_APPEND))
		{
			if (!current->next || (current->next->type != TOKEN_ARGUMENT
					&& current->next->type != TOKEN_COMMAND))
			{
				printf(ERR_SYNTAX);
				free_token_list(tokens);
				return (NULL);
			}
		}
		if (current->type == TOKEN_PIPE && !current->next)
		{
			printf(ERR_SYNTAX);
			free_token_list(tokens);
			return (NULL);
		}
		current = current->next;
	}
	return (tokens);
}

t_token	*parse_input(char *input, int *code)
{
	t_token	*token_list;

	if (!input)
	{
		*code = 2;
		return (NULL);
	}
	token_list = lexing(input);
	if (!token_list)
	{
		*code = 2;
		return (NULL);
	}
	return (token_list);
}

t_token	*lexing(char *input)
{
	t_token	*tokens;
	char	*processed_input;

	if (!input || !*input)
		return (NULL);
	processed_input = handle_escape_char(input);
	if (!processed_input)
		return (NULL);
	tokens = tokenize_input(processed_input);
	free(processed_input);
	if (!tokens)
		return (NULL);
	return (validate_tokens(tokens));
}
