/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/31 16:09:08 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction strdup() renvoie un pointeur sur une nouvelle chaîne de 
caractères qui est dupliquée depuis s. */

#include "libft.h"

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
		return (NULL);
	i = 0;
	while (i < len)
	{
		src[i] = s[i];
		i++;
	}
	src[i] = '\0';
	return (src);
}

// char	*ft_strdup(const char *s)
// {
// 	char	*src;
// 	int		len;

// 	len = 0;
// 	while (s[len])
// 	{
// 		len++;
// 	}
// 	src = (char *) malloc(sizeof(char) * (len + 1));
// 	if (!src)
// 	{
// 		return (NULL);
// 	}
// 	src[len] = '\0';
// 	while (len >= 0)
// 	{
// 		src[len] = s[len];
// 		len--;
// 	}
// 	return (src);
// }
/*
#include <stdio.h>

int	main(void)
{
	char	str[12] = "123";

	printf("%s\n", str);
	printf("%s\n", ft_strdup(str));
	return (0);
}*/
