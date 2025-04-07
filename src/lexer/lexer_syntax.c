/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:29:51 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/07 21:58:02 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(char *input)
{
	if (input[0] == '<')
		return (0);
	if ((input[0] == '\\' && input[1] == '\\') || input[0] == '\\')
	{
		printf(ERR_CMD, input);
		return (-1);
	}
	if (input[0] == ';')
		return (print_error_message(ERR_SYNTAX));
	if (input[0] == '-')
	{
		printf(ERR_CMD, input);
		return (-1);
	}
	if (input[0] == '!' || input[0] == ':')
		return (-1);
	return (0);
}

int	delimiter_error(char *input)
{
	if (input[0] == '&' && input[1] != '&')
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if (input[0] == '&' && input[1] == '&' && input[2] == '&')
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if (input[0] == '|' && input[1] == '|' && input[2] == '|')
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if ((input[0] == '/' || input[0] == '.') && (input[1] == '/'
			|| input[1] == '.'))
	{
		printf(ERR_DIR, input);
		return (-1);
	}
	return (0);
}

int	character_error(char *input)
{
	if (input[0] == '~')
	{
		printf(ERR_DIR, getenv("HOME"));
		return (-1);
	}
	if (input[0] == '*')
	{
		printf("bash: Applications: command not found\n");
		return (-1);
	}
	if ((input[0] == '\'' && input[1] == '\'' && input[2] == '\0')
		|| (input[0] == '"' && input[1] == '"' && input[2] == '\0'))
		return (print_error_message("bash: Applications: command not found\n"));
	if (input[0] == '(' && (input[1] == '(' || input[1] == ')'))
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if (input[0] == '/' && (input[1] == '\0' || input[1] == ' '))
	{
		printf(ERR_DIR, input);
		return (-1);
	}
	return (0);
}

int	input_check(char *input)
{
	if (input[0] == TOKEN_ARGUMENT && input[1] == '\0')
	{
		printf(ERR_CMD, input);
		return (-1);
	}
	return (0);
}
