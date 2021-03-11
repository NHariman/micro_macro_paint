#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_zone
{
	int w, h;
	char bg;
}			t_zone;

typedef struct s_rr
{
	float x, y, w, h;
	char type, chr;
}			t_rr;

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

int ft_err(char *s)
{
	write(1, s, ft_strlen(s));
	return (1);
}

int	check_base(t_zone *zone)
{
	if (zone->w <= 0 || zone->h <= 0 || zone->w > 300 || zone->h > 300)
		return (1);
	return (0);
}

char *get_bg(FILE *file, t_zone *zone)
{
	char *bg;
	int ret;

	ret = fscanf(file, "%i %i %c\n", &zone->w, &zone->h, &zone->bg);
	if (ret != 3)
		return (NULL);
	if (check_base(zone))
		return (NULL);
	bg = (char *)malloc(sizeof(char) * zone->w * zone->h);
	int i = 0;
	while (i < zone->h * zone->w)
	{
		bg[i] = zone->bg;
		i++;
	}
	return (bg);
}

int check_shape(float y, float x, t_rr rr)
{
	if ((y < rr.y || y > rr.y + rr.h) || (x < rr.x || x > rr.x + rr.w))
		return (0);
	if ((x - rr.x < 1.000000 || (rr.x + rr.w) - x < 1.00000) ||
		(y - rr.y < 1.000000 || (rr.y + rr.h) - y < 1.00000))
		return (2);
	return (1);
}

void draw_shapes(char **draw, t_rr rr, t_zone zone)
{
	int x = 0;
	int y = 0;
	int ret = 0;

	while (y < zone.h)
	{
		x = 0;
		while (x < zone.w)
		{
			ret = check_shape((float)y, (float)x, rr);
			if ((rr.type == 'r' && ret == 2) || (rr.type == 'R' && ret))
				(*draw)[(y * zone.w) + x] = rr.chr;
			x++;
		}
		y++;
	}
}

int	get_shapes(FILE *file, char **draw, t_zone zone)
{
	int ret = 6;
	t_rr rr;

	while (ret == 6)
	{
		ret = fscanf(file, "%c %f %f %f %f %c\n", &rr.type, &rr.x, &rr.y, &rr.w, &rr.h, &rr.chr);
		if (ret == -1)
			break ;
		if ((ret != 6 && ret != -1) || rr.w <= 0.000000 || rr.h <= 0.00000)
			return (1);
		if (rr.type != 'r' && rr.type != 'R')
			return (1);
		draw_shapes(draw, rr, zone);
	}
	return (0);
}

void draw_draw(char *draw, t_zone zone)
{
	int i = 0;
	while (i < zone.h)
	{
		write(1, draw + (i * zone.w), zone.w);
		write(1, "\n", 1);
		i++;
	}
}

int main(int ac, char **av)
{
	FILE *file;
	char *draw;
	t_zone zone;

	if (ac != 2)
		return(ft_err("Error: argument\n"));
	if ((file = fopen(av[1], "r")) == NULL)
		return(ft_err("Error: Operation file corrupted\n"));
	if ((draw = get_bg(file, &zone)) == NULL)
	{
		fclose(file);
		return(ft_err("Error: Operation file corrupted\n"));
	}
	if (get_shapes(file, &draw, zone))
	{
		free(draw);
		fclose(file);
		return(ft_err("Error: Operation file corrupted\n"));
	}
	draw_draw(draw, zone);
	free(draw);
	fclose(file);
	return (0);
}