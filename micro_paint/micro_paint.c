#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_shape
{
	int w, h;
	char bg;
}				t_shape;

typedef struct s_rr
{
	float x, y, w, h;
	char r, chr;
}				t_rr;


int		ft_strlen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}


int		ft_err(char *s)
{
	write(1, s, ft_strlen(s));
	return(1);
}

void	print_image(char *s, t_shape *shape)
{
	int i = 0;
	while (i < shape->h)
	{
		write(1, s + (shape->w * i), shape->w);
		write(1, "\n", 1);
		i++;
	}
}

int		get_file(FILE **file, char *s)
{
	*file = fopen(s, "r");
	if (!*file)
		return (1);
	return (0);
}

int		ft_err_free(FILE *file, char *zone, char *s)
{
	if (file)
		fclose(file);
	if (zone)
		free(zone);
	write(1, s, ft_strlen(s));
	return (1);
}

char	*get_zone(FILE *file, t_shape *shape)
{
	char *zone;
	int ret;

	ret = fscanf(file, "%i %i %c\n", &shape->w, &shape->h, &shape->bg);
	if (ret != 3)
		return (NULL);
	if (shape->w <= 0 || shape->h <= 0 || shape->w > 300 || shape->h > 300)
		return (NULL);
	zone = (char *)malloc(sizeof(char) * (shape->w * shape->h));
	if (!zone)
		return (NULL);
	int i = 0;
	while (i < (shape->w * shape->h))
	{
		zone[i] = shape->bg;
		i++;
	}
	return (zone);
}

int		in_rectangle(t_rr *rr, float x, float y)
{
	if (x < rr->x || y < rr->y || (rr->x + rr->w) < x || (rr->y + rr->h) < y)
		return (0);
	if ((x - rr->x) < 1.000000 || (y - rr->y) < 1.00000 || ((rr->x + rr->w - x) < 1.00000) || ((rr->y + rr->h - y) < 1.000000))
		return (2);
	return (1);
}

void	draw_shape(char **zone, t_shape *shape, t_rr *rr)
{
	int y = 0;
	int x = 0;
	int ret = 0;
	while (y < shape->h)
	{
		x = 0;
		while (x < shape->w)
		{
			ret = in_rectangle(rr, (float)x, (float)y);
			if ((rr->r == 'r' && ret == 2) || (rr->r == 'R' && ret))
				(*zone)[(y * shape->w) + x] = rr->chr;
			x++;
		}
		y++;
	}
}

int		get_shapes(FILE *file, char **zone, t_shape *shape)
{
	int 	ret;
	t_rr 	rr;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rr.r, &rr.x, &rr.y, &rr.w, &rr.h, &rr.chr)) == 6)
	{
		if (rr.w <= 0 || rr.h <= 0 || (rr.r != 'R' && rr.r != 'r'))
			return (1);
		// if (ret == -1)
		// 	return (0);
		//printf("%c\n", rr.chr);
		draw_shape(zone, shape, &rr);
	}
	if (ret != -1)
		return (1);
	return (0);
}

int		main(int ac, char **av)
{
	FILE *file;
	char *zone;
	t_shape shape;
	if (ac != 2)
		return(ft_err("Error: argument\n"));
	if (get_file(&file, av[1]))
		return(ft_err("Error: Operation file corrupted\n"));
	zone = get_zone(file, &shape);
	if (!zone)
		return(ft_err_free(file, zone, "Error: Operation file corrupted1\n"));
	if (get_shapes(file, &zone, &shape))
		return(ft_err_free(file, zone, "Error: Operation file corrupted2\n"));
	print_image(zone, &shape);
	fclose(file);
	free(zone);
	return (0);
}