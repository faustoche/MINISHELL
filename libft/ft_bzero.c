/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:45:09 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  La fonction bzero() met à 0 (octets contenant « \0 »)
les n premiers octets du bloc pointé par s.   */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{	
	while (n--)
	{
		*(unsigned char *)s = 0;
		s++;
	}
}
/*
int	main(void)
{
	char str[40] = "Faustiiiiine";
	int	n = 0;
	
	ft_bzero(str, 2);
	while (n < 14)
	{
		printf("%d", str[n]);
		n++;
	}
	return (0);
}*/
