/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Convert hex digit to int value
*/
int	hex_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (c - 'A' + 10);
}

/*
** Parse hex digits after "0x" prefix
*/
long	parse_hex_value(char *str, int *i)
{
	long	result;

	result = 0;
	*i += 2;
	while (str[*i] && str[*i] != ',' && str[*i] != '\n')
	{
		if (str[*i] >= '0' && str[*i] <= '9')
			result = result * 16 + (str[*i] - '0');
		else if ((str[*i] >= 'a' && str[*i] <= 'f')
			|| (str[*i] >= 'A' && str[*i] <= 'F'))
			result = result * 16 + hex_to_int(str[*i]);
		else
			break ;
		(*i)++;
	}
	return (result);
}

/*
** Convert hexadecimal string to integer (uses ft_atoi for decimals)
** Handles "0xff", "0xFF", "123" formats
*/
int	ft_atoi_hex(char *str)
{
	int		i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		return (sign * parse_hex_value(str, &i));
	return (ft_atoi(str));
}
