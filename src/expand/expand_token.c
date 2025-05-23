/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:58:53 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 10:10:04 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variable_in_token(t_token *token, t_env *env_list, int *code)
{
	char	*expanded;

	if (token->type == SINGLE_QUOTE)
		return ;
	if (token->value && ft_strchr(token->value, '$'))
	{
		expanded = expand_variable(env_list, token->value, code);
		if (expanded)
		{
			free(token->value);
			token->value = expanded;
		}
	}
}

void	expand_tokens(t_token *token_list, t_env *env_list, int	*exit_code)
{
	t_token	*token;
	t_lexer	temp_lexer;

	token = token_list;
	temp_lexer.tokens = token_list;
	while (token)
	{
		expand_variable_in_token(token, env_list, exit_code);
		token = token->next;
	}
}

int	expand_check_digit(t_expand *exp)
{
	if (isdigit(exp->str[exp->i]))
	{
		exp->i++;
		return (1);
	}
	return (0);
}
