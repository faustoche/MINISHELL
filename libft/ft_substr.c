/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:48:19 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Alloue (avec malloc(3)) et retourne une chaîne de
caractères issue de la chaîne ’s’.
Cette nouvelle chaîne commence à l’index ’start’ et
a pour taille maximale ’len’*/

#include "libft.h"

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
/*
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*src;
	int		len;

	len = 0;
	while (s[len])
	{
		len++;
	}
	src = (char *) malloc(sizeof(char) * (len + 1));
	if (!src)
	{
		return (NULL);
	}
	src[len] = '\0';
	while (len >= 0)
	{
		src[len] = s[len];
		len--;
	}
	return (src);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	if (n == 0)
	{
		return (i);
	}
	while ((n > 1) && (*src != '\0'))
	{
		*dst = *src;
		dst++;
		src++;
		n--;
	}
	*dst = '\0';
	return (i);
}

int	main(void)
{
	char str[] = "slttoulemondecskizi";
	char *str_sec = ft_substr(str, 8, 5);
	printf("extract: %s\n", str_sec);
	free(str_sec);
}*/
