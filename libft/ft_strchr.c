/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 07:53:58 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:25:41 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	to_find;

	i = 0;
	to_find = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == to_find)
		return ((char *)&s[i]);
	return (NULL);
}
/*
int main(void)
{
    char *s = "bonjour";
    char attendu[] = "jour";
    char *resultat = ft_strchr(s, 'j');

    printf("Attendu : \"%s\"\n", attendu);
    printf("Obtenu  : \"%s\"\n", resultat);

    return 0;
}
*/
