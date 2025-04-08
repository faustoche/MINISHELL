/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:06:19 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 18:07:08 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote_errors(t_lexer *lexer, char *word, int end)
{
	if (lexer->input[end] == '\'' || lexer->input[end] == '"')
	{
		if (lexer->command)
		{
			printf(ERR_CMD, word);
			free(word);
			return (-1);
		}
	}
	return (0);
}

char	*create_final_word(t_lexer *lexer, char *word, int start, int end)
{
	if (!word)
		return (ft_strndup(lexer->input + start, end - start));
	return (word);
}
