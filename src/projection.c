/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Absolute value
*/
int	abs_value(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

/*
** Transform map coordinates to screen coordinates
** Isometric projection
*/
void	project_point(t_point *pt, int z, t_window *win)
{
	int	old_x;
	int	old_y;

	old_x = pt->x * win->zoom;
	old_y = pt->y * win->zoom;
	pt->x = (old_x - old_y) * 0.866;
	pt->y = (old_x + old_y) * 0.5 - z;
	pt->x += WIDTH / 2;
	pt->y += HEIGHT / 2;
}
