/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:58:53 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/31 15:17:31 by fcrocq           ###   ########.fr       */
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

/* Ajoute les tokens supplémentaires trouvés par l'expansion de wildcards */

void	add_wildcard_tokens(t_lexer *lexer, char **matches, int start_index)
{
	int	i;

	i = start_index;
	while (matches[i])
	{
		add_token(lexer, matches[i], ft_strlen(matches[i]), TOKEN_ARGUMENT);
		i++;
	}
}

/* Expanse les wildcards dans un token */

void	expand_wildcard_in_token(t_token *token, t_lexer *lexer)
{
	char	**matches;

	if (token->value && (ft_strchr(token->value, '*')
			|| ft_strchr(token->value, '?')))
	{
		matches = expand_wildcards(token->value);
		if (matches && matches[0])
		{
			free(token->value);
			token->value = ft_strdup(matches[0]);
			add_wildcard_tokens(lexer, matches, 1);
		}
		free_wildcards(matches);
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
		expand_wildcard_in_token(token, &temp_lexer);
		token = token->next;
	}
}
