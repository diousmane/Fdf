/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:06:55 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:43:02 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_isset(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*result;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && ft_isset(s1[start], set))
		start++;
	end = ft_strlen((char *)s1);
	while (end > start && ft_isset(s1[end - 1], set))
		end--;
	result = (char *)malloc(end - start + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1 + start, end - start);
	result[end - start] = '\0';
	return (result);
}
/*
int main(void)
{
    char *s = "  \t42 Paris\t  ";
    char *set = " \t";
    char *attendu = "42 Paris";
    char *resultat = ft_strtrim(s, set);

    printf("Attendu : \"%s\"\n", attendu);
    printf("Obtenu  : \"%s\"\n", resultat);

    free(resultat);
    return 0;
}
*/
