/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:47:02 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  La fonction strchr() renvoie un pointeur sur la première occurrence du 
caractère c dans la chaîne s. */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}	
	if (*s == (char)c)
		return ((char *)s);
	else
		return (0);
}
/*
#include<stdio.h>

int	main(void)
{
	char str[] = "Faustoche";
	char *ptr = ft_strchr(str, 'c');

	printf("%c", *ptr);
}*/
