#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_bg
{
	int w, h;
	char bg;
}			t_bg;

typedef struct s_cc
{
	float x, y, r;
	char type, chr;
}			t_cc;

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int		ft_err(char *s)
{
	write(1, s, ft_strlen(s));
	return (1);
}

char *get_bg(FILE *file, t_bg *bg)
{
	char *zone;
	int ret = fscanf(file, "%i %i %c\n", &bg->w, &bg->h, &bg->bg);
	if (ret != 3)
		return (NULL);
	if (bg->w <= 0 || bg->h <= 0 || bg->w > 300 || bg->h > 300)
		return (NULL);
	zone = (char *)malloc(sizeof(char) * bg->w * bg->h);
	if (!zone)
		return (NULL);
	int i = 0;
	while (i < bg->h * bg->w)
	{
		zone[i] = bg->bg;
		i++;
	}
	return (zone);
}

int check_circle(float y, float x, t_cc cc)
{
	float dist = sqrtf(powf(x - cc.x, 2.) + powf(y - cc.y, 2.));

	if (dist > cc.r)
		return (0);
	if (cc.r - dist < 1.00000)
		return (2);
	return (1);
}

void draw_circle(char **draw, t_bg bg, t_cc cc)
{
	int x = 0;
	int y = 0;
	int ret;
	while (y < bg.h)
	{
		x = 0;
		while (x < bg.w)
		{
			ret = check_circle((float)y, (float)x, cc);
			if ((cc.type == 'c' && ret == 2) || (cc.type == 'C' && ret))
				(*draw)[(y * bg.w) + x] = cc.chr;
			x++;
		}
		y++;
	}
}

int get_shapes(FILE *file, t_bg bg, char **draw)
{
	int ret = 5;
	t_cc cc;

	while (ret == 5)
	{
		ret = fscanf(file, "%c %f %f %f %c\n", &cc.type, &cc.x, &cc.y, &cc.r, &cc.chr);
		if (ret == -1)
			break ;
		if (ret != 5)
			return (1);
		if (cc.r <= 0.000000 || (cc.type != 'c' && cc.type != 'C'))
			return (1);
		draw_circle(draw, bg, cc);
	}
	return (0);
}

void draw_shapes(char *draw, t_bg bg)
{
	int i = 0;
	while (i < bg.h)
	{
		write(1, draw + (i * bg.w), bg.w);
		write(1, "\n", 1);
		i++;
	}
}

int main(int ac, char **av)
{
	FILE *file;
	t_bg bg;
	char *draw;

	if (ac != 2)
		return (ft_err("Error: argument\n"));
	if ((file = fopen(av[1], "r")) == NULL)
		return (ft_err("Error: Operation file corrupted\n"));
	if ((draw = get_bg(file, &bg)) == NULL)
	{
		fclose(file);
		return (ft_err("Error: Operation file corrupted\n"));
	}
	if (get_shapes(file, bg, &draw))
	{
		free(draw);
		fclose(file);
		return (ft_err("Error: Operation file corrupted\n"));
	}
	draw_shapes(draw, bg);
	free(draw);
	fclose(file);
	return (0);
}