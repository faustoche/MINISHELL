/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:32:49 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/10 11:49:45 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *init_minimal_env(void)
{
    t_env   *env_list;
    t_env   *new_var;
    char    pwd[PATH_MAX];

    env_list = NULL;
    if (!getcwd(pwd, sizeof(pwd)))
        return (NULL);
    new_var = malloc(sizeof(t_env));
    if (!new_var)
        return (NULL);
    new_var->name = ft_strdup("PWD");
    new_var->value = ft_strdup(pwd);
    new_var->next = NULL;
    env_list = new_var;
    new_var = malloc(sizeof(t_env));
    if (!new_var)
        return (NULL);
    new_var->name = ft_strdup("SHLVL");
    new_var->value = ft_strdup("1");
    new_var->next = NULL;
    ft_lstadd_back(&env_list, new_var);
    new_var = malloc(sizeof(t_env));
    if (!new_var)
        return (NULL);
    new_var->name = ft_strdup("_");
    new_var->value = ft_strdup("usr/bin/env");
    new_var->next = NULL;
    ft_lstadd_back(&env_list, new_var);
    return (env_list);
}

