/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:25:23 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/19 21:50:44 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main function to expand variables */

char	*expand_variable(t_env *env_list, char *str, int quote_type)
{
	t_expand	exp;

	if (quote_type == SINGLE_QUOTE)
		return (ft_strdup(str));
	exp.env_list = env_list;
	if (!init_expand_result(str, &exp))
		return (NULL);
	if (!expand_loop(&exp))
		return (NULL);
	return (exp.result);
}

char	*init_expand_result(const char *str, t_expand *exp)
{
	exp->capacity = ft_strlen(str) + 1;
	exp->result = malloc(exp->capacity);
	exp->i = 0;
	exp->j = 0;
	exp->str = (char *)str;
	if (!exp->result)
		return (NULL);
	return (exp->result);
}

/* Main expansion loop */

int	expand_loop(t_expand *exp)
{
	while (exp->str[exp->i])
	{
		if (!check_buffer_size(exp))
			return (0);
		if (exp->str[exp->i] == '$' && exp->str[exp->i + 1] && isalpha(exp->str[exp->i + 1]))
		{
			if (!process_variable(exp))
				return (0);
		}
		else
			exp->result[(exp->j)++] = exp->str[(exp->i)++];
	}
	exp->result[exp->j] = '\0';
	return (1);
}

/* Expand les variables dans un token */

void expand_variable_in_token(t_token *token, t_env *env_list)
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

void add_wildcard_tokens(t_lexer *lexer, char **matches, int start_index)
{
	int i;

	i = start_index;
	while (matches[i])
	{
		add_token(lexer, matches[i], ft_strlen(matches[i]), TOKEN_ARGUMENT);
		i++;
	}
}

/* Expanse les wildcards dans un token */

void expand_wildcard_in_token(t_token *token, t_lexer *lexer)
{
	char **matches;

	if (token->value && (ft_strchr(token->value, '*') || ft_strchr(token->value, '?')))
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

void expand_tokens(t_token *token_list, t_env *env_list)
{
	t_token *token;
	t_lexer temp_lexer;

	token = token_list;
	temp_lexer.tokens = token_list;
	while (token)
	{
		expand_variable_in_token(token, env_list);
		expand_wildcard_in_token(token, &temp_lexer);
		token = token->next;
	}
}
