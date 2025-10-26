/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 00:55:00 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:41:25 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_wordcount(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

char	*ft_worddup(const char *s, int start, int len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + start, len + 1);
	return (word);
}

int	ft_split_fill(char **tab, const char *s, char c)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	while (s[j])
	{
		while (s[j] == c)
			j++;
		if (s[j])
		{
			len = 0;
			while (s[j + len] && s[j + len] != c)
				len++;
			tab[i] = ft_worddup(s, j, len);
			if (!tab[i])
				return (0);
			i++;
			j += len;
		}
	}
	tab[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		words;

	if (!s)
		return (NULL);
	words = ft_wordcount(s, c);
	tab = malloc((words + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	if (!ft_split_fill(tab, s, c))
		return (NULL);
	return (tab);
}
/*
void	free_tab(char **tab)
{
    int i = 0;
    if (!tab)
        return;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

int main(void)
{
    char *s = "un-deux-trois";
    char separateur = '-';
    char **resultat = ft_split(s, separateur);

    printf("Obtenu : \"%s\" \"%s\" \"%s\"\n", resultat[0],
	resultat[1], resultat[2]);

    free_tab(resultat);

    return 0;
}
*/
