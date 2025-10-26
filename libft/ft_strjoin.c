/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:40:00 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:41:40 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	char	*dest_s2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen((char *)s1);
	len2 = ft_strlen((char *)s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	dest_s2 = result + len1;
	ft_memcpy(dest_s2, s2, len2);
	result[len1 + len2] = '\0';
	return (result);
}
/*
int main(void)
{
    char *s1 = "Hello, ";
    char *s2 = "world!";
    char *attendu = "Hello, world!";
    char *resultat = ft_strjoin(s1, s2);

    printf("Attendu : \"%s\"\n", attendu);
    printf("Obtenu  : \"%s\"\n", resultat);

    free(resultat);
    return 0;
}
*/
