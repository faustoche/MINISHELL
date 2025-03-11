/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:25:23 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/07 11:53:43 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tilde(char *input)
{
	char	*home;
	char	*result;

	if (!input || input[0] != '~')
		return (ft_strdup(input));
	home = getenv("HOME");
	if (!home)
		return (ft_strdup(input));
	result = malloc(ft_strlen(home) + ft_strlen(input));
	if (!result)
		return (NULL);
	ft_strcpy(result, home);
	strcat(result, input + 1); // mettre le vrai depuis la libft
	return (result);
}

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

void	expand_tokens(t_token *token_list, t_env *env_list)
{
	t_token	*token;
	char	*expanded;

	token = token_list;
	while (token)
	{
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
		token = token->next;
	}
}
