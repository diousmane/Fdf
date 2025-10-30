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

int	is_valid_integer(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] && str[i] != '\n')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	free_split(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	read_and_fill_rows(int fd, t_map *map)
{
	char	*line;
	int		y;

	y = 0;
	line = get_next_line(fd);
	while (line && y < map->height)
	{
		if (y > 0 && count_columns(line) != map->width)
			return (free(line), 0);
		if (!process_line(line, map, y))
			return (free(line), 0);
		free(line);
		y++;
		line = get_next_line(fd);
	}
	return (1);
}
