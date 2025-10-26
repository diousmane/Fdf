/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 08:55:42 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:25:44 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

char	*ft_strdup(char *src)
{
	int		i;
	char	*dup;

	i = 0;
	while (src[i] != '\0')
		i++;
	dup = (char *)malloc((i + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
/*
int main(void)
{
    char *src = "Libft 42";
    char *attendu = "Libft 42";
    char *resultat = ft_strdup(src);

    printf("Attendu : \"%s\"\n", attendu);
    printf("Obtenu  : \"%s\"\n", resultat);

    free(resultat);
    return 0;
}
*/
