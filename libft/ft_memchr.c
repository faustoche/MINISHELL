/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:20 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction memchr() examine les n premiers octets de la zone mémoire 
pointée par s à la recherche du caractère c. Le premier octet correspondant 
à c (interprété comme un unsigned char) arrête l'opération. */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
	{
		if (*ptr == (unsigned char)c)
		{
			return (ptr);
		}
		ptr++;
	}
	return (NULL);
}
/*
#include<stdio.h>

int	main(void)
{
	char str[] = "Faustohe";
	char *ptr = ft_memchr(str, 'c', sizeof(str));
	if (ptr != NULL)
	{
		printf("%c\n", *ptr);
	}
	else
		printf("Pas de correspondance\n");
}*/
