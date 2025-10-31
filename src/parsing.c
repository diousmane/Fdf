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
** Check if line width matches expected width
*/
int	check_line_width(char *line, int expected_width)
{
	int	current_width;

	current_width = count_columns(line);
	return (current_width == expected_width);
}

/*
** Get map dimensions and validate rectangular shape
** Returns 1 on success, 0 on error
*/
int	get_map_dimensions(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	int		first_width;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	map->height = 0;
	first_width = 0;
	line = get_next_line(fd);
	while (line)
	{
		map->height++;
		if (map->height == 1)
			first_width = count_columns(line);
		else if (!check_line_width(line, first_width))
			return (free(line), close(fd), 0);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	map->width = first_width;
	return (map->height > 0 && map->width > 0);
}

/*
** Process one line: validate fields and fill row
*/
int	process_line(char *line, t_map *map, int y)
{
	char	**numbers;
	int		x;
	int		i;

	numbers = ft_split(line, ' ');
	if (!numbers)
		return (0);
	x = 0;
	i = 0;
	while (numbers[i])
	{
		if (!is_valid_integer(numbers[i]))
			return (free_split(numbers), 0);
		if (x < map->width)
			map->z_matrix[y][x++] = parse_fdf_value(numbers[i]);
		i++;
	}
	free_split(numbers);
	while (x < map->width)
		map->z_matrix[y][x++] = 0;
	return (1);
}

/*
** Clear zmatrix
*/
void	clear_zmatrix(t_map *map)
{
	int	i;

	if (!map || !map->z_matrix)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->z_matrix[i])
			free(map->z_matrix[i]);
		i++;
	}
	free(map->z_matrix);
	map->z_matrix = NULL;
}

/*
** Main parsing function: get dimensions, allocate, and fill map
*/
int	parse_map(char *filename, t_map *map)
{
	int		fd;
	int		ok;

	if (!get_map_dimensions(filename, map))
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		return (0);
	}
	if (!allocate_matrices(map))
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		clear_zmatrix(map);
		return (0);
	}
	ok = read_and_fill_rows(fd, map);
	close(fd);
	if (!ok)
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		clear_zmatrix(map);
		return (0);
	}
	return (1);
}
