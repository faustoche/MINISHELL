/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:50:10 by faustoche         #+#    #+#             */
/*   Updated: 2025/02/26 11:59:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Gérer le contenu entre guillememets */

int	single_quotes(t_lexer *lexer, int i)
{
	int		start;
	char	*word;
	int		type;

	start = i++;
	while (lexer->input[i] && lexer->input[i] != '\'')
		i++;
	if (lexer->input[i] == '\'')
	{
		word = ft_strndup(lexer->input + start + 1, i - start - 1);
		if (!word)
			return (-1);
		type = get_token_type(word, &lexer->command);
		add_token(lexer, word, i - start - 1, type);
		free(word);
		return (i + 1);
	}
	return (0);
}

int	double_quotes(t_lexer *lexer, int i)
{
	int		start;
	char	*word;
	int		type;

	start = i++;
	while (lexer->input[i] && !(lexer->input[i] == '"'
			&& lexer->input[i - 1] != '\\'))
		i++;
	if (lexer->input[i] == '"')
	{
		word = ft_strndup(lexer->input + start + 1, i - start - 1);
		if (!word)
			return (-1);
		type = get_token_type(word, &lexer->command);
		add_token(lexer, word, i - start - 1, type);
		free(word);
		return (i + 1);
	}
	return (0);
}
