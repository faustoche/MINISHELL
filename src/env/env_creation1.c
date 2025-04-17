/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:32:49 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/13 20:50:10 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_pwd(void)
{
	t_env	*new_var;
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, sizeof(pwd)))
		return (NULL);
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->name = ft_strdup("PWD");
	new_var->value = ft_strdup(pwd);
	new_var->next = NULL;
	return (new_var);
}

static t_env	*init_shlvl(t_env *env_list)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->name = ft_strdup("SHLVL");
	new_var->value = ft_strdup("1");
	new_var->next = NULL;
	ft_lstadd_back(&env_list, new_var);
	return (env_list);
}

static t_env	*init_user(t_env *env_list)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->name = ft_strdup("_");
	new_var->value = ft_strdup("usr/bin/env");
	new_var->next = NULL;
	ft_lstadd_back(&env_list, new_var);
	return (env_list);
}

t_env	*init_minimal_env(void)
{
	t_env	*env_list;

	env_list = init_pwd();
	env_list = init_shlvl(env_list);
	env_list = init_user(env_list);
	return (env_list);
}
