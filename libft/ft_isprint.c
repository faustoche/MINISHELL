/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:45:39 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* verifie s'il s'agit d'un caractere imprimable y compris l'espace*/

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
/*
int	main(void)
{
	printf("%d\n", ft_isprint('H'));
	printf("%d\n", ft_isprint('8'));
	printf("%d\n", ft_isprint('\n'));
	printf("%d\n", ft_isprint(' '));
	printf("%d\n", ft_isprint(-86));
}
*/