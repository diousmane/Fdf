/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <math.h>
# include <fcntl.h>

# define WIDTH		1920
# define HEIGHT		1080
# define TITLE		"FdF - ousdial"
# define KEY_ESC	65307
# define COLOR_WHITE	0xFFFFFF

/*
** Map structure
*/
typedef struct s_map
{
	int		**z_matrix;
	int		width;
	int		height;
}	t_map;

/*
** Window structure
*/
typedef struct s_window
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_data;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_map		*map;
	int			zoom;
}	t_window;

/*
** Point structure
*/
typedef struct s_point
{
	int		x;
	int		y;
}	t_point;

/*
** Parsing
*/
int			parse_map(char *filename, t_map *map);
int			count_lines(char *filename);
int			count_columns(char *line);
int			fill_map(char *filename, t_map *map);
int			process_line(char *line, t_map *map, int y);
int			allocate_matrices(t_map *map);
void		draw_connections(t_window *win, t_point *p1, int x, int y);

/*
** Parsing utils (validation)
*/
int			is_valid_integer(char *str);
void		free_split(char **arr);
int			read_and_fill_rows(int fd, t_map *map);

/*
** Projection
*/
int			abs_value(int n);
void		project_point(t_point *pt, int z, t_window *win);

/*
** Drawing
*/
void		draw_map(t_window *win);
void		draw_line(t_window *win, t_point *p1, t_point *p2);
void		line_horizontal(t_window *win, t_point *p1, t_point *p2);
void		line_vertical(t_window *win, t_point *p1, t_point *p2);
void		put_pixel(t_window *win, int x, int y, int color);

/*
** Display
*/
void		init_mlx(t_window *win);
void		calc_zoom(t_window *win);
int			key_press(int key, t_window *win);
int			close_win(t_window *win);

/*
** Utils
*/
void		free_map(t_map *map);
void		clear_zmatrix(t_map *map);
int			get_width(char *filename, t_map *map);

/*
** Main
*/
int			check_extension(char *filename);
int			start_fdf(char *filename);

#endif
