/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:04:05 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/09 14:48:21 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env *unset_utils(t_env *env_list, t_env *new_env_list, char *name)
{
    // Vérifier si le nom est NULL ou une chaîne vide
    if (name == NULL || *name == '\0')
    {
        printf(ERR_EXP, name);
        free_env_list(&env_list);
        return (new_env_list);
    }

    if (!is_valid_identifier(name))
    {
        printf(ERR_EXP, name);
        free_env_list(&env_list);
        return (new_env_list);
    }
    return (NULL);
}

t_env *ft_unset(t_env *env_list, char *name)
{
    t_env *new_env_list;
    t_env *current;
    t_env *prev;

    // Vérifier si le nom est NULL ou une chaîne vide
    if (name == NULL || *name == '\0')
    {
        printf(ERR_EXP, name);
        return (env_list);
    }

    new_env_list = copy_env_list(env_list);
    current = new_env_list;
    prev = NULL;

    while (current)
    {
        t_env *result = unset_utils(env_list, new_env_list, name);
        if (result != NULL)
            return (result);

        if (strcmp(current->name, name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                new_env_list = current->next;
            
            free(current->name);
            free(current->value);
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    free_env_list(&env_list);
    return (new_env_list);
}
