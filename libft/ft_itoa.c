/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:45:42 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Alloue (avec malloc(3)) et retourne une chaîne
de caractères représentant l’entier ’n’ reçu en
argument. Les nombres négatifs doivent être gérés. */

#include "libft.h"

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = (char *)malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	return (tmp);
}	

static int	int_len(long nbr)
{
	int	count;

	count = 0;
	if (nbr <= 0)
	{
		count++;
	}
	while (nbr != 0)
	{
		count++;
		nbr /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	nbr;

	nbr = n;
	len = int_len(nbr);
	result = pre_conv(len);
	if (!result)
		return (NULL);
	if (nbr < 0)
		nbr = -nbr;
	i = len - 1;
	if (nbr == 0)
		result[i--] = '0';
	while (nbr != 0)
	{
		result[i--] = ((nbr % 10) + '0');
		nbr = nbr / 10;
	}
	if (n < 0)
		result[0] = '-';
	result[len] = '\0';
	return (result);
}
/*
int	main(void)
{
	printf("%s\n", ft_itoa(96957));
}
*/