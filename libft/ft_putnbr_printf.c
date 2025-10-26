/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 07:07:26 by ousdiall          #+#    #+#             */
/*   Updated: 2025/10/01 16:10:49 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_print(int n)
{
	long	nb;
	int		count;

	nb = n;
	count = 0;
	if (nb < 0)
	{
		ft_putchar_print('-');
		count++;
		nb = -nb;
	}
	if (nb >= 10)
		count += ft_putnbr_print(nb / 10);
	count += ft_putchar_print((nb % 10) + '0');
	return (count);
}
