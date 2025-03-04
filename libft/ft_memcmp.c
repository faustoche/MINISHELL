/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:23 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction memcmp() compare les n premiers octets des zones mémoire
 s1 et s2. Elle renvoie un entier inférieur, égal, ou supérieur à zéro, 
 si s1 est respectivement inférieure, égale ou supérieur à s2.  */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n--)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}
/*
int	main(void)
{
	char str1[] = "Faustine";
	char str2[] = "Faustine";
	int result = ft_memcmp(str1, str2, sizeof(str1));
	printf("Diff : %d", result);
}*/
