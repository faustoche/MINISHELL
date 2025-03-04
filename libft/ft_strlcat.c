/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:47:43 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The strlcat() function appends the NUL-terminated string src to 
the end of dst. It will append at most size - strlen(dst) - 1 bytes, 
NUL-terminating the result.  */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	d_len;
	unsigned int	s_len;

	i = 0;
	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	if (size <= d_len)
	{
		return (s_len + size);
	}
	while ((src[i] != '\0') && (i < size - d_len - 1))
	{
		dst[d_len + i] = src[i];
		i++;
	}
	dst[d_len + i] = '\0';
	return (d_len + s_len);
}
/*

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	main(void)
{
	char dest[40] = "Helllllo";
	char src[] = "faustoche";
	ft_strlcat(dest, src, sizeof(dest));
	printf("%s\n", dest);
}*/
