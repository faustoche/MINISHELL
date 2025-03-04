/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/04 16:09:26 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_element(char *env)
{
	t_env	*element;
	char	*pos_equal;

	if (!env)
		return (NULL);
	element = malloc(sizeof(t_env)); // leak ici
	if (!element)
		return (NULL);
	element->name = NULL;
	element->value = NULL;
	element->next = NULL;
	pos_equal = ft_strchr(env, '=');
	if (!pos_equal)
	{
		element->name = ft_strdup(env);
		element->value = ft_strdup("");
	}
	else
	{
		element->name = ft_strndup(env, pos_equal - env); // leak ici
		element->value = ft_strdup(pos_equal + 1); // leak ici
	}
	if (!element->name || !element->value)
	{
		if (element->name)
			free(element->name);
		if (element->value)	
			free(element->value);
		free(element);
		return (NULL);
	}
	element->next = NULL;
	return (element);
}

char *expand_variable(t_env *env_list, char *str)
{
    size_t	result_capacity;
	char	*result;
    size_t	i;
	size_t	j;
    char	*name;
	char	*value;
    size_t	len;

	i = 0;
	j = 0;
	result_capacity = ft_strlen(str) + 1;
	result = malloc(result_capacity);
    if (!str || !result)
        return NULL;
    while (str[i])
    {
        if (j + 1 >= result_capacity)
        {
            result_capacity *= 2;
            char *temp = realloc(result, result_capacity);
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
                    char *temp = realloc(result, result_capacity);
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
    return result;
}

void expand_tokens(t_token *token_list, t_env *env_list)
{
    t_token	*token;
    char	*expanded;
    
    token = token_list;
    while (token)
    {
        if (token->value && ft_strchr(token->value, '$') && token->type != SINGLE_DEL)
        {
            expanded = expand_variable(env_list, token->value);
            if (expanded)
            {
                free(token->value);
                token->value = expanded;
            }
        }
        token = token->next;
    }
}

int execute_env_command(t_cmd *cmd, t_env *env_list)
{
    int		i;
    char	*expanded;
	
	i = 0;
    while (cmd->args[i])
    {
        if (ft_strchr(cmd->args[i], '$'))
        {
            expanded = expand_variable(env_list, cmd->args[i]);
            if (expanded)
            {
                free(cmd->args[i]);
                cmd->args[i] = expanded;
            }
        }
        i++;
    }
    return (0);
}
