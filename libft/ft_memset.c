/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/07 20:14:38 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  La fonction memset() remplit les n premiers octets de la zone 
mémoire pointée par s avec l'octet c.    */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	temp;
	unsigned char	*char_ptr;

	char_ptr = (unsigned char *)s;
	while (n--)
	{
		temp = (unsigned char)c;
		*char_ptr = temp;
		char_ptr++;
	}
	return (s);
}
/*
int	main(void)
{
	char str[40] = "Faustiiiiine";
	ft_memset(str, 'F', 8);
	printf("%s\n", str);
	return (0);
}*/
