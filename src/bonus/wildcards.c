/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:35:02 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/19 21:44:15 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s)
{
	char	*src;
	int		len;
	int		i;

	len = 0;
	while (s[len])
		len++;
	src = (char *) malloc(sizeof(char) * (len + 1));
	if (!src)
	{
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		src[i] = s[i];
		i++;
	}
	src[i] = '\0';
	return (src);
}

int match_wildcard(char *sign, char *name)
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

int collect_matches(char **matches, DIR *dir, char *sign)
{
    struct dirent *entry;
    int count;

    count = 0;
    while ((entry = readdir(dir)))
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
