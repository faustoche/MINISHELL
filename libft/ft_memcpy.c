/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:27 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction memcpy() copie n octets depuis la zone mémoire src vers la 
zone mémoire dest. Les deux zones ne doivent pas se chevaucher */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*ptr_dest;

	ptr_dest = dest;
	if (!dest && !src)
		return (dest);
	while (n--)
		*(char *)dest++ = *(char *)src++;
	return (ptr_dest);
}
/*
int	main(void)
{
	char	src[] = "Salulatimcskizi";
	char	dest[5];
	ft_memcpy(dest, src, sizeof(src));
	printf("Dest : %s", dest);
}*/
