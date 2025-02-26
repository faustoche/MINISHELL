/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:29:51 by faustoche         #+#    #+#             */
/*   Updated: 2025/02/26 13:46:08 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

CA A INTEREET A FONCTIONNER JE DEVIENS FOU */




int	syntax_error(char *input)
{
	if ((input[0] == '\\' && input[1] == '\\') || input[0] == '\\')
	{
		printf(ERR_CMD, input);
		return (-1);
	}
	if (input[0] == ';')
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if (input[0] == '-')
	{
		printf(ERR_CMD, input);
		return (-1);
	}
	if ((input[0] == '/' || input[0] == '.') && (input[1] == '/' || input[1] == '.'))
	{
		printf(ERR_DIR, input);
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
	if (input[0] == '|' && input[1] != '|')
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if ((input[0] == '&' && input[1] == '&' && input[2] == '&')
		|| (input[0] == '&' && input[1] == '&' && input[2] == '\0'))
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	if ((input[0] == '|' && input[1] == '|' && input[2] == '|')
		|| (input[0] == '|' && input[1] == '|' && input[2] == '\0'))
	{
		printf(ERR_SYNTAX);
		return (-1);
	}
	return (0);
}

int	character_error(char *input)
{
	if (input[0] == '~')
	{
		printf("bash: $PATH:is a directory\n");
		return (-1);
	}
	if (input[0] == '*')
	{
		printf("bash: Applications: command not found\n");
		return (-1);
	}
	if (input[0] == '\'' && input[1] == '\'')
	{
		printf("bash: Applications: command not found\n");
		return (-1);
	}
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

int	string_error(t_token *token)
{
	if (token && token->type == TOKEN_ARGUMENT)
	{
		printf("bash: command not found\n");
		return (-1);
	}
	while (token && token->type)
	{
		token = token->next;
	}
	return (0);
}
