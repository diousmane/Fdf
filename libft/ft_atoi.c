/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:05:00 by ousdiall          #+#    #+#             */
/*   Updated: 2025/06/04 12:38:40 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
/*
int main(void)
{
    printf("%d\n", ft_atoi("   -42"));        // -42
    printf("%d\n", ft_atoi("   +123abc"));    // 123
    printf("%d\n", ft_atoi("  2147483647"));  // 2147483647
    printf("%d\n", ft_atoi("  -2147483648")); // -2147483648
    printf("%d\n", ft_atoi("  0"));           // 0
    printf("%d\n", ft_atoi("  +0"));          // 0
    printf("%d\n", ft_atoi("  --42"));        // 0
    printf("%d\n", ft_atoi("abc"));           // 0
    return 0;
}
*/
