/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:48:13 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  La fonction strrchr() renvoie un pointeur sur la dernière occurrence 
du caractère c dans la chaîne s. */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char const	*temp;

	temp = 0;
	while (*s)
	{
		if (*s == (char)c)
			temp = s;
		s++;
	}	
	if (*s == (char)c)
		return ((char *)s);
	else
		return ((char *)temp);
}
/*
int	main(void)
{
	char str[] = "Faustoe";
	char *ptr = ft_strrchr(str, 'c');

	if (ptr == NULL)
	{
		printf("Caractere non trouve\n");
	}
	else
	{
		printf("%c", *ptr);
	}
}*/
