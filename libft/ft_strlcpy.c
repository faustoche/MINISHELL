/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:47:46 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The strlcpy() function copies up to size - 1 characters from the 
NUL-terminated string src to dst, NUL-terminating the result.  */

#include "libft.h"

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
/*
#include<stdio.h>

int	main(void)
{
	char	src[] = "Faustine";
	char	dest[9];

	ft_strlcpy(dest, src, sizeof(dest));
	printf("Dest : %s\n", dest);
}*/
