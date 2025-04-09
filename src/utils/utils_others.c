/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_others.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:01:52 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/09 10:01:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

int	handle_direct_env_var(char *input, t_env *env_list)
{
	if (!input || input[0] != '$')
		return (0);
	if (input[1] == '\0')
		return (0);
		
	char	*var_value;
	
	var_value = find_var_value(env_list, input + 1);
	if (var_value)
	{
		printf("%s\n", var_value);
		return (1);
	}
	return (0);
}