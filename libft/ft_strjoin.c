/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:47:39 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Alloue (avec malloc(3)) et retourne une nouvelle
chaîne, résultat de la concaténation de s1 et s2.*/

#include "libft.h"

static char	*str_new(size_t n)
{
	char	*str;

	str = (char *)malloc (sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*str_ptr;

	if (!s1 && !s2)
		return (NULL);
	str = str_new(ft_strlen(s1) + ft_strlen(s2));
	if (!str)
		return (NULL);
	str_ptr = str;
	while (*s1)
	{
		*str++ = *s1++;
	}
	while (*s2)
	{
		*str++ = *s2++;
	}
	*str = '\0';
	return (str_ptr);
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

int	main(void)
{
	char str1[] = "Faustoche ";
	char str2[] = "Pistoche";
	char *result = ft_strjoin(str1, str2);
	printf("Join : %s\n", result);	
	free(result);
}
*/