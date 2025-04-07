/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:25:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/06 18:31:12 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char *fix_dollar_quote(char *input)
{
	char *result;
	int i = 0, j = 0;
	int in_single_quotes = 0;
	int in_double_quotes = 0;
	
	result = malloc(strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && !in_single_quotes && 
			(input[i+1] == '"' || input[i + 1] == '\''))
		{
			char quote = input[i + 1];
			i += 2;
			while (input[i] && input[i] != quote)
				result[j++] = input[i++];
			if (input[i] == quote)
				i++;
		}
		else if (input[i] == '\'')
		{
			in_single_quotes = !in_single_quotes;
			result[j++] = input[i++];
		}
		else if (input[i] == '"')
		{
			in_double_quotes = !in_double_quotes;
			result[j++] = input[i++];
		}
		else if ((input[i] == '"' && input[i - 1] == '"') || 
				 (input[i] == '\'' && input[i - 1] == '\''))
			result[j++] = input[i++];
		else if (input[i] == '$' && in_double_quotes && 
				 input[i + 1] == '"' && input[i - 1] == '"')
		{
			result[j++] = '$';
			i++;
			if (input[i] == '"')
				i++;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	if (j == 0 || ft_strcmp(result, input) == 0)
	{
		char	*dup = ft_strdup(input);
		free(result);
		return (dup);
	}
	return (result);
}
