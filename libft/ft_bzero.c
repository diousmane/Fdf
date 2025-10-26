/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:14:01 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:21:30 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
/*
int main(void)
{
    char str[10] = "abcdefghi";
    printf("Avant bzero : %s\n", str);
    ft_bzero(str + 2, 4);
    printf("Après bzero : ");
    for (int i = 0; i < 10; i++)
    {
        if (str[i])
            printf("%c", str[i]);
        else
            printf("\\0");
    }
    printf("\n");
    return 0;
}
*/
