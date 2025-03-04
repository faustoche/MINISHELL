/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:31 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Cette fonction permet de copier un bloc de mémoire spécifié par le 
paramètre source dans un nouvel emplacement désigné par le paramètre 
destination. Un chevauchement des deux zones est possible. */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*ptr_destination;
	{
		ptr_destination = dest;
		if (!dest && !src)
			return (dest);
		if (dest == src)
			return (dest);
		if (dest > src)
		{
			while (n--)
				((char *)dest)[n] = ((char *)src)[n];
		}
		else
		{
			while (n--)
				*(char *)dest++ = *(char *)src++;
		}
		return (ptr_destination);
	}
}
/*
int	main(void)
{
	char	src[];
	src[] = "Salulatimcskizi";
	char	dest[5];
	ft_memmove(dest, src, sizeof(src));
	printf("Dest : %s", dest);
}*/
