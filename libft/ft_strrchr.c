/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:50:00 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:42:22 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
/*
int main(void)
{
    char *s = "bonjour bonjour";
    char attendu = 'j';
    char *resultat = ft_strrchr(s, 'j');

    printf("Attendu : \"%s\"\n", "jour");
    printf("Obtenu  : \"%s\"\n", resultat);

    return 0;
}
*/
