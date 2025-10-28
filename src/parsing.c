/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    ft +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Main parsing function
*/
int	parse_map(char *filename, t_map *map)
{
	int		fd;
	char	*first_line;

	map->height = count_lines(filename);
	if (map->height == 0)
	{
		ft_putstr_fd("Error: empty file\n", 2);
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: cannot open file\n", 2);
		return (0);
	}
	first_line = get_next_line(fd);
	if (!first_line)
		return (0);
	map->width = count_columns(first_line);
	free(first_line);
	close(fd);
	if (map->width == 0)
		return (0);
	fill_map(filename, map);
	return (1);
}

/*
** Free an entire map
*/
void	free_map(t_map *map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->z_matrix && map->z_matrix[i])
			free(map->z_matrix[i]);
		if (map->color_matrix && map->color_matrix[i])
			free(map->color_matrix[i]);
		i++;
	}
	if (map->z_matrix)
		free(map->z_matrix);
	if (map->color_matrix)
		free(map->color_matrix);
	free(map);
}
