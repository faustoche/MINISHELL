/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:48:09 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The strnstr() function locates the first	occurrence of the  null-termi-
       nated  string little in the string big, where not more than len charac-
       ters are	searched.  Characters that appear after	a `\0'	character  are
       not  searched.   */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (big == NULL && len == 0)
		return (NULL);
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && little[j] != '\0' && i < len)
	{
		j = 0;
		if (big[i] == little[0])
		{
			while (big[i + j] == little[j] && (i + j) < len)
			{
				if (little[j + 1] == '\0')
					return ((char *)big + i);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	char str1[] = "Faoche";
	char str2[] = "Faust";
	char *result = ft_strnstr(str1, str2, sizeof(str1));
	printf("Diff : %s", result);
}*/
