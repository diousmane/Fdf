# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/05 12:51:12 by lharrar           #+#    #+#              #
#    Updated: 2025/10/26 18:47:15 by ousdiall         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_isprint.c \
ft_memchr.c \
ft_memcmp.c \
ft_memcpy.c \
ft_memmove.c \
ft_memset.c \
ft_strchr.c \
ft_strdup.c \
ft_strlcat.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strrchr.c \
ft_tolower.c \
ft_toupper.c \
ft_substr.c \
ft_strjoin.c \
ft_strtrim.c \
ft_split.c \
ft_itoa.c \
ft_strmapi.c \
ft_striteri.c \
ft_putchar_fd.c \
ft_putstr_fd.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_printf.c \
ft_check.c \
ft_putchar_printf.c \
ft_puthexa_printf.c \
ft_puthexa_upper_printf.c \
ft_putnbr_printf.c \
ft_putstr_printf.c \
ft_putunsigned_printf.c \
ft_check.c \
get_next_line_utils.c \
get_next_line.c \

SRCS_B = ft_lstnew_bonus.c \
ft_lstadd_front_bonus.c \
ft_lstsize_bonus.c \
ft_lstlast_bonus.c \
ft_lstadd_back_bonus.c \
ft_lstdelone_bonus.c \
ft_lstclear_bonus.c \
ft_lstiter_bonus.c \
ft_lstmap_bonus.c


INC = libft.h

OBJS = $(SRCS:.c=.o)
OBJS_B = $(SRCS_B:.c=.o)
CC = cc
CFLAGS = -Wall -Werror -Wextra -I. -g
AR = ar rcs
RM = rm -f

all : $(NAME)

$(NAME) : $(OBJS)
	@$(AR) $(NAME) $(OBJS)

%.o: %.c
		@$(CC) $(CFLAGS) -c $< -o $@

bonus : $(OBJS) $(OBJS_B)
	@$(AR) $(NAME) $(OBJS_B)

clean :
	@$(RM) $(OBJS) $(OBJS_B)

fclean :
	@$(RM) $(NAME) $(OBJS) $(OBJS_B)

re : fclean all

.PHONY : all clean fclean re bonus
