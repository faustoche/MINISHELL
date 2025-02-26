/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:50:49 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the list */

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

/* Split l'entrée utilisateur en tokens */

t_token	*parse_input(char *input)
{
	t_token	*token_list;

	if (!input)
		return (NULL);

	token_list = lexing(input);
	if (!token_list)
		return (NULL);
	return (token_list);
}
	
/* Used only for debugging and testing */
	
void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: %-10s | Type: %d\n", head->value, head->type);
		head = head->next;
	}
}

/*

if (// erreur de syntaxe de token->list)
{
	printf("bash: syntax error near unexpected token\n");
	free_token_list(token_list);
	return (NULL);
}
return token_list;

*/