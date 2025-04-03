/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:58:53 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/03 22:49:52 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Expand les variables dans un token */

void	expand_variable_in_token(t_token *token, t_env *env_list)
{
	char	*expanded;

	if (token->value && ft_strchr(token->value, '$')
		&& token->type != SINGLE_QUOTE)
	{
		expanded = expand_variable(env_list, token->value, token->type);
		if (expanded)
		{
			free(token->value);
			token->value = expanded;
		}
	}
}

/* Fonction principale pour l'expansion des tokens */

void	expand_tokens(t_token *token_list, t_env *env_list)
{
	t_token	*token;
	t_lexer	temp_lexer;

	token = token_list;
	temp_lexer.tokens = token_list;
	while (token)
	{
		expand_variable_in_token(token, env_list);
		token = token->next;
	}
}
