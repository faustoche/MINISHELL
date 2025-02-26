/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:57:41 by faustoche         #+#    #+#             */
/*   Updated: 2025/02/26 11:54:51 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int handle_export(t_lexer *lexer, int i)
{
    int start;
    char *key;
    char *value;

	start = i;
	while (lexer->input[i] && lexer->input[i] != '=')
		i++;
	if (lexer->input[i] == '=')
	{
		key = ft_strndup(lexer->input + start, i - start);
		start = i++;
		while (lexer->input[i] && !is_separator(lexer->input[i]))
			i++;
		value = ft_strndup(lexer->input + start, i - start);
		add_token(lexer, key, strlen(key), TOKEN_EXPORT_KEY);
		add_separator_token(lexer);
		add_token(lexer, value, strlen(value), TOKEN_EXPORT_VALUE);
		free(key);
		free(value);
	}
	return (i);
}
*/