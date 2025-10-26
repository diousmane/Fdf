/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 08:24:15 by ousdiall          #+#    #+#             */
/*   Updated: 2025/10/01 16:23:08 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_pointer(void *ptr)
{
	int	count;

	count = 0;
	if (!ptr)
		count += ft_putstr_print("(nil)");
	else
	{
		count += ft_putstr_print("0x");
		count += ft_puthexa_print((unsigned long)ptr);
	}
	return (count);
}

int	ft_check(char c, va_list args)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar_print((char)va_arg(args, int));
	else if (c == 's')
		count += ft_putstr_print(va_arg(args, char *));
	else if (c == '%')
		count += ft_putchar_print('%');
	else if (c == 'd' || c == 'i')
		count += ft_putnbr_print(va_arg(args, int));
	else if (c == 'u')
		count += ft_putunsigned_print(va_arg(args, unsigned int));
	else if (c == 'x')
		count += ft_puthexa_print(va_arg(args, unsigned int));
	else if (c == 'X')
		count += ft_puthexa_upper_print(va_arg(args, unsigned int));
	else if (c == 'p')
		count += ft_check_pointer(va_arg(args, void *));
	else
	{
		return (-1);
	}
	return (count);
}
