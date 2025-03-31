/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_separator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:08:09 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/31 15:18:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Identification des délimiteur priorites - pas encore utilisée */

int	double_delimiter(char *input, int i)
{
	if (input[i] == '&' && input[i + 1] == '&')
		return (-1);
	if (input[i] == '|' && input[i + 1] == '|')
		return (-1);
	return (0);
}

/* Verifier si l'index est un delimitateur */

int	handle_delimiter(t_lexer *lexer, int i)
{
	char	*delim;
	int		len;
	int		type;

	len = 1;
	if ((lexer->input[i] == '>' && lexer->input[i + 1] == '>')
		|| (lexer->input[i] == '<' && lexer->input[i + 1] == '<')
		|| (lexer->input[i] == '&' && lexer->input[i + 1] == '&')
		|| (lexer->input[i] == '|' && lexer->input[i + 1] == '|'))
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
