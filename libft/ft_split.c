/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:58 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Alloue (avec malloc(3)) et retourne un tableau
de chaînes de caractères obtenu en séparant ’s’ à
l’aide du caractère ’c’, utilisé comme délimiteur.
Le tableau doit être terminé par NULL. */

#include "libft.h"

static char	ft_count_word(const char *str, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (*str)
	{
		if (*str != c && !i)
		{
			i = 1;
			count++;
		}
		else if (*str == c)
		{
			i = 0;
		}
		str++;
	}
	return (count);
}

static void	free_str(size_t i, char **str)
{
	while (i > 0)
	{
		i--;
		free(*(str + i));
	}
	free(str);
}

static size_t	word_size(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*(s + i) && *(s + i) != c)
		i++;
	return (i);
}

static char	**other_split(char const *s, char c, char **str, size_t words_count)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < words_count)
	{
		while (*(s + j) && *(s + j) == c)
			j++;
		*(str + i) = ft_substr(s, j, word_size(&*(s + j), c));
		if (!*(str + i))
		{
			free_str(i, str);
			return (NULL);
		}
		while (*(s + j) && *(s + j) != c)
			j++;
		i++;
	}
	*(str + i) = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		words;

	if (!s)
		return (NULL);
	words = ft_count_word(s, c);
	str = (char **)malloc(sizeof(char *) * (words + 1));
	if (!str)
		return (NULL);
	str = other_split(s, c, str, words);
	return (str);
}
/*

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*cpy;
	size_t	len_s;

	if (!s)
	{
		return (NULL);
	}
	len_s = ft_strlen(s);
	if (start < len_s)
	{
		if (len_s - start < len)
			len = len_s - start;
		cpy = (char *)malloc((len + 1) * sizeof(char));
		if (!cpy)
			return (NULL);
		ft_strlcpy(cpy, s + start, len + 1);
	}
	else
		return (ft_strdup(""));
	return (cpy);
}

int	main(void)
{
	char str[] = "je mange des chats";
	char	**result = ft_split(str, ' ');
	int i = 0;
	while (result[i] != NULL)
	{
		printf("%s\n", result[i]);
		free(result[i]);
		i++;
	}
	free(result);
	return (0);
}
*/