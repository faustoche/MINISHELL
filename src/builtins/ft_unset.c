/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:04:05 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/24 20:29:38 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void unset_variable(char *name, t_env *env_list)
// {
//     t_env	*current;
//     t_env	*previous;
	
// 	previous = NULL;
// 	current = env_list;
//     while (current)
//     {
//         if (ft_strcmp(current->name, name) == 0)
//         {
//             if (previous)
//                 previous->next = current->next;
//             else
//                 env_list = current->next;
//             free(current->name);
//             free(current->value);
//             free(current);
//             return;
//         }
//         previous = current;
//         current = current->next;
//     }
// }
