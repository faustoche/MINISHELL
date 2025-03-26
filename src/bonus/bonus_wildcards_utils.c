/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_wildcards_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:11:06 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/26 18:11:48 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_matches(char **matches, int count, DIR *dir)
{
	while (count >= 0)
	{
		free(matches[count]);
		count--;
	}
	free(matches);
	if (dir)
		closedir(dir);
}

int	add_match(char **matches, int count, char *name)
{
	matches[count] = ft_strdup(name);
	if (!matches[count])
		return (0);
	return (-1);
}

void	free_wildcards(char **matches)
{
	int	i;

	i = 0;
	if (!matches)
		return ;
	while (matches[i])
	{
		free(matches[i]);
		i++;
	}
	free(matches);
}
