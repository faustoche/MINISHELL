/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:47:57 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction strcmp() compare les n premiers octets des deux chaînes 
s1 et s2. */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (n > 0 && *s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
/*
#include<stdio.h>
int	main(void)
{
	char str1[] = "Faustoche";
	char str2[] = "Faustine";
	int result = ft_strncmp(str1, str2, sizeof(str1));
	printf("Diff : %d", result);
}*/
