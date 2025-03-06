/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/06 09:01:28 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_element(char *env)
{
	t_env	*element;
	char	*pos_equal;

	if (!env)
		return (NULL);
	element = malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	pos_equal = ft_strchr(env, '=');
	if (!pos_equal)
	{
		element->name = ft_strdup(env);
		element->value = ft_strdup("");
	}
	else
	{
		element->name = ft_strndup(env, pos_equal - env);
		element->value = ft_strdup(pos_equal + 1);
	}
	if (!element->name || !element->value)
        free_elements(element);
	element->next = NULL;
	return (element);
}

char *expand_variable(t_env *env_list, char *str, int quote_type)
{
    size_t	result_capacity;
	char	*result;
    size_t	i;
	size_t	j;
    char	*name;
	char	*value;
    size_t	len;
    char    *temp;

	i = 0;
	j = 0;
	result_capacity = ft_strlen(str) + 1;
	result = malloc(result_capacity);
    if (!str || !result)
    return NULL;
    if (quote_type == SINGLE_QUOTE)
        return (ft_strdup(str));
    while (str[i])
    {
        if (j + 1 >= result_capacity)
        {
            result_capacity *= 2;
            temp = ft_realloc(result, result_capacity);
            if (!temp)
            {
                free(result);
                return NULL;
            }
            result = temp;
        }
        if (str[i] == '$' && str[i + 1] && isalpha(str[i + 1]))
        {
            i++;
            len = 0;
            while (str[i + len] && (isalnum(str[i + len]) || str[i + len] == '_'))
                len++;
            name = ft_strndup(str + i, len);
            if (!name)
            {
                free(result);
                return NULL;
            }
            value = get_env_value(env_list, name);
            if (value)
            {
                size_t value_len = ft_strlen(value);

                while (j + value_len + 1 >= result_capacity)
                {
                    result_capacity *= 2;
                    temp = realloc(result, result_capacity); // Ici changer par realloc perso mais attention conditional jumps
                    if (!temp)
                    {
                        free(result);
                        free(name);
                        return NULL;
                    }
                    result = temp;
                }
                strcpy(result + j, value);
                j += value_len;
            }
            free(name);
            i += len;
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

void expand_tokens(t_token *token_list, t_env *env_list)
{
    t_token	*token;
    char	*expanded;
    
    token = token_list;
    while (token)
    {
        if (token->value && ft_strchr(token->value, '$') && token->type != SINGLE_QUOTE)
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

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current = env_list;

	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}	
	return (getenv(name));
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*current;
	t_env	*new_element;
	int		i;

	env_list = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_element = create_env_element(envp[i]);
		if (!new_element)
		{
			free_env_list(env_list);
			return (NULL);
		}
		if (!env_list)
		{
			env_list = new_element;
			current = env_list;
		}
		else
		{
			current->next = new_element;
			current = new_element;
		}
		i++;
	}
	return (env_list);
}
