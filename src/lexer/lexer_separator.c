/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_separator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:08:09 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/01 16:05:36 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifier si l'index est un delimitateur */

int	handle_delimiter(t_lexer *lexer, int i)
{
	char	*delim;
	int		len;
	int		type;

	len = 1;
	if ((lexer->input[i] == '>' && lexer->input[i + 1] == '>')
		|| (lexer->input[i] == '<' && lexer->input[i + 1] == '<'))
		len = 2;
	delim = ft_strndup(lexer->input + i, len);
	if (!delim)
	{
		free(delim);
		return (-1);
	}
	type = get_token_type(delim, &lexer->command);
	add_token(lexer, delim, len, type);
	free(delim);
	if (type == TOKEN_PIPE || type == TOKEN_SEPARATOR)
		lexer->command = 1;
	return (i + len);
}

/* Gestion des caractères d'échappement */

// creation d'une nouvelle chaine qui est la chaine traite sans les caracteres d'echappement

char	*handle_escape_char(char *input)
{
	char	*processed;
	int		i;
	int		j;
	int		len;
	int		singles;
	int		doubles;

	len = ft_strlen(input);
	singles = 0;
	doubles = 0;
	i = 0;
	j = 0;
	processed = malloc(sizeof(char) * (len + 1));
	if (!processed)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\'' && !doubles)
		{
			if (singles == 0)
				singles = 1;
			else
				singles = 0;
			processed[j++] = input[i++];
		}
		else if (input[i] == '"' && !singles)
		{
			if (doubles == 0)
				doubles = 1;
			else
				doubles = 0;
			processed[j++] = input[i++];
		}
		else if (input[i] == '\\' && (i + 1) < len)
		{
			if (singles)
				processed[j++] = input[i++];
			else if (doubles)
			{
				if (input[i + 1] == '"' || input[i + 1] == '\\' || input[i + 1] == '$')
				{
					i++;
					processed[j++] = input[i++];
				}
				else
					processed[j++] = input[i++];
			}
			else
			{
				i++;
				processed[j++] = input[i++];
			}
		}
		else
		{
			processed[j++] = input[i++];
		}
	}
	processed[j] = '\0';
	return (processed);
}
