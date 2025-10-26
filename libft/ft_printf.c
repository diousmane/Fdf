/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:30:42 by ousdiall          #+#    #+#             */
/*   Updated: 2025/10/01 16:33:23 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_format(const char *str, int *i, va_list args)
{
	int	result;

	if (!str[*i + 1])
	{
		va_end(args);
		return (-1);
	}
	result = ft_check(str[*i + 1], args);
	if (result == -1)
	{
		va_end(args);
		return (-1);
	}
	*i += 2;
	return (result);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		count;
	int		result;

	if (!str)
		return (-1);
	va_start(args, str);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			result = handle_format(str, &i, args);
			if (result == -1)
				return (va_end(args), -1);
			count += result;
		}
		else
			count += ft_putchar_print(str[i++]);
	}
	va_end(args);
	return (count);
}
