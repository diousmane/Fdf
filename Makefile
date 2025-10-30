NAME = fdf

SRC = src/main.c \
	  src/parsing.c \
	  src/parse_utils.c \
	  src/parse_utils2.c \
	  src/projection.c \
	  src/drawing.c \
	  src/draw_line.c \
	  src/display.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT = libft/libft.a
MLX = minilibx-linux/libmlx.a
INCLUDES = -I includes -I libft -I minilibx-linux
LIBS = -L libft -lft -L minilibx-linux -lmlx -lXext -lX11 -lm

all: $(NAME)

$(LIBFT):
	@echo "Compiling libft..."
	@make -C libft

$(MLX):
	@echo "Compiling MinilibX..."
	@make -C minilibx-linux

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	@echo "Compiling fdf..."
	@$(CC) $(OBJ) $(LIBS) -o $(NAME)
	@echo "fdf created successfully!"

clean:
	@echo "Cleaning object files..."
	@make -C libft clean
	@make -C minilibx-linux clean
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleaning fdf..."
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
