/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:53:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split(char *input)
{
	char	**token;
	int		i;
	int		count;
	int		token_count;

	i = 0;
	count = 0;
	token_count = count_tokens(input);
	token = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!token)
		return (NULL);
	while (input[i])
	{
		if (is_space(input[i]))
			i = handle_space(input, i);
		else if (is_separator(input[i]))
			i = handle_separator(token, &count, input, i);
		else if (input[i] == '\'' || input[i] == '"')
			i = handle_quotes(token, &count, input, i);
		else
			i = check_word(token, &count, input, i);
		if (i == -1)
		{
			free_tokens(token);
			return (NULL);
		}
	}
	token[count] = NULL;
	return (token);
}

/* Count how many token thus how many args and commands*/

int	count_tokens(char *input)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i = handle_space(input, i);
		else if (is_separator(input[i++]))
			count++;
		else if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] == quote)
				i++;
			count++;
		}
		else
		{
			while (input[i] && !is_space(input[i]) && !is_separator(input[i])
				&& input[i] != '\'' && input[i] != '"')
				i++;
			count++;
		}
	}
	return (count);
}
/*
** Vérifie s'il s'agit d'un séparateur ou non
*/

int	is_separator(int c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';'
		|| c == '&' || c == '(' || c == ')');
}

/*
** Vérifie s'il s'agit d'un espace ou non
*/

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

/*
** Saute les espaces si nécessaire
*/

int	handle_space(char *input, int i)
{
	while (input[i] && is_space(input[i]))
		i++;
	return (i);
}

/*
** Veirifier si c'est > si oui, check si i + 1 est aussi > 
** Ajouter le separateur au tableau de tokens
** AvANCEr de 1 ou 2 
*/

int	handle_separator(char **tokens, int *count, char *input, int i)
{
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			input_to_tokens(tokens, count, ">>", 2);
			return (i + 2);
		}
		else
		{
			input_to_tokens(tokens, count, ">", 1);
			return (i + 1);
		}	
	}
	else if (input[i] == '<')
	{
		if (input[i + 1] == '<')
		{
			input_to_tokens(tokens, count, "<<", 2);
			return (i + 2);
		}
		else
		{
			input_to_tokens(tokens, count, "<", 1);
			return (i + 1);
		}
	}
	else if (input[i] == '|')
	{
		input_to_tokens(tokens, count, "|", 1);
		return (i + 1);
	}
	else if (input[i] == ';')
	{
		input_to_tokens(tokens, count, ";", 1);
		return (i + 1);
	}
	return (i);
}

/*
** Ajouter un mot aka token à un tableau de char
** Si la longueur du mot est inférieur alors on quitte
** Malloc de la taille du token
** Strcpy du début à début + longueur dans le tableau
** Penser à la terminaison null
*/

void	input_to_tokens(char **tokens, int *count, char *start, int length)
{
	char	*token;

	if (length <= 0)
		return ;
	token = (char *)malloc(sizeof(char) * (length + 1));
	if (!token)
		return ;
	ft_strncpy(token, start, length);
	token[length] = '\0';
	tokens[*count] = token;
	(*count)++;
}

/* */

int	handle_quotes(char **tokens, int *count, char *input, int i)
{
	char	quote;
	int		start;

	quote = input[i++];
	start = i;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == '\0')
		return (-1);
	input_to_tokens(tokens, count, input + start, i - start);
	return (i + 1);
}

int	check_word(char **tokens, int *count, char *input, int i)
{
	int	start;

	start = i;
	while (input[i] && !is_space(input[i]) && !is_separator(input[i])
		&& input[i] != '\'' && input[i] != '"')
		i++;
	input_to_tokens(tokens, count, input + start, i - start);
	return (i);
}
