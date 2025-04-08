/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:25:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 23:20:35 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialize_vars(int *index, int *sq, int *dq)
{
	index[0] = 0;
	index[1] = 0;
	*sq = 0;
	*dq = 0;
}

static int	handle_dollar_in_dq(char *input, char *result, int *index, int dq)
{
	if (input[index[0]] == '$' && dq
		&& input[index[0] + 1] == '"' && input[index[0] - 1] == '"')
	{
		index[0] = handle_dollar_dq(input, result, index[0], &index[1]);
		return (1);
	}
	return (0);
}

static void	process_char(char *input, char *result, int *index, int *quotes)
{
	if (handle_dollar_case(input, result, index, quotes[0]))
		return ;
	if (handle_quote_case(input, result, index, quotes))
		return ;
	if (handle_edge_quotes(input, result, index))
		return ;
	if (handle_dollar_in_dq(input, result, index, quotes[1]))
		return ;
	result[index[1]++] = input[index[0]++];
}

static char	*process_input(char *input, char *result)
{
	int	index[2];
	int	quotes[2];

	initialize_vars(index, &quotes[0], &quotes[1]);
	while (input[index[0]])
		process_char(input, result, index, quotes);
	result[index[1]] = '\0';
	return (result);
}

char	*fix_dollar_quote(char *input)
{
	char	*dup;
	char	*result;

	result = malloc(strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	result = process_input(input, result);
	if (strlen(result) == 0 || ft_strcmp(result, input) == 0)
	{
		dup = ft_strdup(input);
		free(result);
		return (dup);
	}
	return (result);
}


// char *fix_dollar_quote(char *input)
// {
// 	char *result;
// 	int i = 0, j = 0;
// 	int in_single_quotes = 0;
// 	int in_double_quotes = 0;

// 	result = malloc(strlen(input) * 2 + 1);
// 	if (!result)
// 		return (NULL);
// 	while (input[i])
// 	{
// 		if (input[i] == '$' && !in_single_quotes && 
// 			(input[i+1] == '"' || input[i + 1] == '\''))
// 		{
// 			char quote = input[i + 1];
// 			i += 2;
// 			while (input[i] && input[i] != quote)
// 				result[j++] = input[i++];
// 			if (input[i] == quote)
// 				i++;
// 		}
// 		else if (input[i] == '\'')
// 		{
// 			in_single_quotes = !in_single_quotes;
// 			result[j++] = input[i++];
// 		}
// 		else if (input[i] == '"')
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			result[j++] = input[i++];
// 		}
// 		else if ((input[i] == '"' && input[i - 1] == '"') || 
// 				 (input[i] == '\'' && input[i - 1] == '\''))
// 			result[j++] = input[i++];
// 		else if (input[i] == '$' && in_double_quotes && 
// 				 input[i + 1] == '"' && input[i - 1] == '"')
// 		{
// 			result[j++] = '$';
// 			i++;
// 			if (input[i] == '"')
// 				i++;
// 		}
// 		else
// 			result[j++] = input[i++];
// 	}
// 	result[j] = '\0';
// 	if (j == 0 || ft_strcmp(result, input) == 0)
// 	{
// 		char	*dup = ft_strdup(input);
// 		free(result);
// 		return (dup);
// 	}
// 	return (result);
// }
