/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:35:02 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/26 18:11:50 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_wildcard(char *sign, char *name)
{
	if (*sign == '\0' && *name == '\0')
		return (-1);
	if (*sign == '\0')
		return (0);
	if (*name == '\0')
	{
		if (*sign == '*')
			return (match_wildcard(sign + 1, name));
		return (0);
	}
	if (*sign == '*')
	{
		if (match_wildcard(sign + 1, name))
			return (-1);
		return (match_wildcard(sign, name + 1));
	}
	if (*sign == '?' || *sign == *name)
		return (match_wildcard(sign + 1, name + 1));
	return (0);
}

char	**init_matches(void)
{
	char	**matches;
	int		i;

	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (NULL);
	i = 0;
	while (i < 1024)
	{
		matches[i] = NULL;
		i++;
	}
	return (matches);
}

char	**handle_no_matches(char *sign)
{
	char	**matches;

	matches = malloc(sizeof(char *) * 2);
	if (!matches)
		return (NULL);
	matches[0] = ft_strdup(sign);
	if (!matches[0])
	{
		free(matches);
		return (NULL);
	}
	matches[1] = NULL;
	return (matches);
}

int	collect_matches(char **matches, DIR *dir, char *sign)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (match_wildcard(sign, entry->d_name))
		{
			if (!add_match(matches, count, entry->d_name))
			{
				clean_matches(matches, count, dir);
				return (-1);
			}
			count++;
		}
	}
	matches[count] = NULL;
	return (count);
}

char	**expand_wildcards(char *sign)
{
	DIR				*dir;
	char			**matches;
	int				count;

	dir = opendir(".");
	if (!dir)
	{
		perror("opendir failed\n");
		return (NULL);
	}
	matches = init_matches();
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	count = collect_matches(matches, dir, sign);
	if (count == -1)
		return (NULL);
	if (count == 0)
	{
		free(matches);
		closedir(dir);
		return (handle_no_matches(sign));
	}
	closedir(dir);
	return (matches);
}
