/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Check if character is a valid hex digit
*/
int	is_hex_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if (c >= 'a' && c <= 'f')
		return (1);
	if (c >= 'A' && c <= 'F')
		return (1);
	return (0);
}

/*
** Validate digits in value part (stops at comma, space, or newline)
*/
int	validate_value_digits(char *str, int *i, int is_hex)
{
	while (str[*i] && str[*i] != ',' && str[*i] != ' '
		&& str[*i] != '\t' && str[*i] != '\n')
	{
		if (is_hex && !is_hex_digit(str[*i]))
			return (0);
		if (!is_hex && (str[*i] < '0' || str[*i] > '9'))
			return (0);
		(*i)++;
	}
	return (1);
}

/*
** Check if string is a valid integer (decimal or hexadecimal)
** Accepts format "value,color" but only validates the value part
** Color part is ignored (no color support in mandatory part)
*/
int	is_valid_integer(char *str)
{
	int		i;
	int		is_hex;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	is_hex = 0;
	if ((str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		&& str[i + 2])
	{
		is_hex = 1;
		i += 2;
	}
	if (!validate_value_digits(str, &i, is_hex))
		return (0);
	return (1);
}
