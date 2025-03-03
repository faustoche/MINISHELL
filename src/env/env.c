/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:42 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/03 20:59:08 by faustoche        ###   ########.fr       */
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
	{
		free_elements(element);
		return (NULL);
	}
	element->next = NULL;
	return (element);
}

char	*expand_variable(t_env *env_list, char *str)
{
	int		i;
	int		j;
	char	*result;
	char	*name;
	char	*value;
	int		len;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) * 2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && isalpha(str[i + 1])) // changer ici
		{
			i++;
			len = 0;
			while (str[i + len] && (isalnum(str[i + len]) || str[i + len] == '_')) // changer ici
				len++;
			name = ft_strndup(str + i, len);
			if (!name)
			{
				free(result);
				return (NULL);
			}
			value = get_env_value(env_list, name);
			free(name);
			if (value)
			{
				strcpy(result + j, value); // ft_strcpy ici
				j += ft_strlen(value);
			}
			i += len;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (ft_realloc(result, j + 1));
}

char	*ft_realloc(char *str, size_t size)
{
    char *new_str;
    
    if (!str)
        return (NULL);
    
    new_str = malloc(size);
    if (!new_str)
    {
        free(str);
        return (NULL);
    }
    
    ft_strncpy(new_str, str, size - 1);
    new_str[size - 1] = '\0';
    free(str);
    
    return (new_str);
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