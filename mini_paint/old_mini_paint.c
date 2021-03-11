#include "micro_paint.h"

int		ft_strlen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

int		ft_err(void)
{
	ft_putstr("Error: Operation file corrupted\n");
	return (1);
}

int		ft_errc(void *ptr)
{
	fclose(ptr);
	return (ft_err());
}

int		is_char(char c)
{
	if (c < 33 && c > 126)
		return (0);
	return (1);
}

void	reset_rr(t_rr *rr)
{
	rr->x = 0;
	rr->y = 0;
	rr->width = 0;
	rr->height = 0;
	rr->type = (char)0;
	rr->c = (char)0;
}

char	**ft_errf(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}

char	*fill_arr(char *s, char c, int width)
{
	int i = 0;
	while (i < width)
	{
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	**make_arr(t_mp *mp)
{
	int i = 0;
	char **arr;

	arr = (char **)malloc(((mp->height + 1) * sizeof(char *)));
	if (!arr)
		return (NULL);
	while (i < mp->height)
	{
		arr[i] = (char *)malloc(((mp->width + 1) * sizeof(char)));
		if (!arr[i])
			return (ft_errf(arr, i));
		arr[i] = fill_arr(arr[i], mp->bgc, mp->width);
		i++;
	}
	arr[i] = (char *)0;
	return (arr);
}

void	ft_printarr(char **arr)
{
	int i = 0;

	while (arr[i] != (char *)0)
	{
		ft_putstr(arr[i]);
		ft_putstr("\n");
		i++;
	}
}

int		fill_open(t_rr *rr, char **arr, t_mp *mp)
{
	int i = 0;
	float x = rr->x;
	float y = rr->y;
	if ((int)x < 0)
		x = 0;
	if ((int)y < 0)
		y = 0;
	float x_end = rr->x + rr->width;
	float y_end = rr->y + rr->height;
	if (rr->x < 0)
		x_end = rr->width + rr->x;
	if ((int)x_end > mp->width)
		x_end = (float)mp->width;
	if (rr->y < 0)
		y_end = rr->height + rr->y;
	if ((int)y_end > mp->height)
		y_end = (float)mp->height;
	while (i <= (int)rr->width - 1 && arr[(int)y][(int)x + i] != '\0')
	{
		arr[(int)y][(int)x + i] = rr->c;
		arr[(int)y_end - 1][(int)x + i] = rr->c;
		i++;
	}
	i = 0;
	while (i <= (int)rr->height - 1 && arr[(int)y + i] != (char *)0)
	{
		arr[(int)y + i][(int)x] = rr->c;
		arr[(int)y + i][(int)x_end - 1] = rr->c;
		i++;
	}
	return (0);
}

int		fill_closed(t_rr *rr, char **arr, t_mp *mp)
{
	int i = 0;
	int j = 0;
	float x = rr->x;
	float y = rr->y;
	if ((int)x < 0)
		x = 0;
	if ((int)y < 0)
		y = 0;
	float x_end = rr->x + rr->width;
	float y_end = rr->y + rr->height;
	if (rr->x < 0)
		x_end = rr->width + rr->x;
	if ((int)x_end > mp->width)
		x_end = (float)mp->width;
	if (rr->y < 0)
		y_end = rr->height + rr->y;
	if ((int)y_end > mp->height)
		y_end = (float)mp->height;
	while (i <= (int)y_end - 1 && arr[(int)y + i] != (char *)0)
	{
		while (j <= (int)x_end - 1 && arr[(int)y][(int)x + j] != '\0')
		{
			arr[(int)y + i][(int)x + j] = rr->c;
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}

int		input_rr(t_rr *rr, char **arr, t_mp *mp)
{
	int ret = 0;
	printf("%c %f %f %f %f %c\n", rr->type, rr->x, rr->y, rr->width, rr->height, rr->c);
	if (rr->type == 'r')
		ret = fill_open(rr, arr, mp);
	else if (rr->type == 'R')
		ret = fill_closed(rr, arr, mp);
	else
		ret = 1;
	return (ret);
}

int		ft_rr(void *ptr, t_mp *mp)
{
	char **arr;
	int scan = 6;
	int ret = 0;
	t_rr *rr;

	arr = make_arr(mp);
	rr = (t_rr *)calloc(1, sizeof(t_rr));
	if (!arr)
		return (1);
	while (scan == 6)
	{
		scan = fscanf(ptr, "%c %f %f %f %f %c\n", &rr->type, &rr->x, &rr->y, &rr->width, &rr->height, &rr->c);
		printf("%c %f %f %f %f %c\nscan: %i\n", rr->type, rr->x, rr->y, rr->width, rr->height, rr->c, scan);
		if (scan == -1)
			break ;
		if (scan < 6 || rr->width <= 0 || rr->height <= 0 || !is_char(rr->c))
		{
			arr = ft_errf(arr, mp->height);
			free(rr);
			return (ft_err());
		}
		ret = input_rr(rr, arr, mp);
		if (ret == 1)
		{
			arr = ft_errf(arr, mp->height);
			free(rr);
			return (ft_err());
		}
		reset_rr(rr);
	}
	ft_printarr(arr);
	arr = ft_errf(arr, mp->height);
	free(rr);
	return (0);
}

int		open_file(char *s)
{
	t_mp *mp;
	int scan = 0;
	void *ptr;
	int ret = 0;
	
	ptr = fopen(s, "r");
	if (ptr == NULL)
		return (ft_err());
	mp = (t_mp *)calloc(1, sizeof(t_mp));
	scan = fscanf(ptr, "%i %i %c\n", &mp->width, &mp->height, &mp->bgc);
	//printf("%i %i %c\n", mp->width, mp->height, mp->bgc);
	if (scan != 3 || (mp->width <= 0 && mp->width > 300) || (mp->height <= 0 && mp->height > 300) ||  !is_char(mp->bgc))
		return(ft_errc(ptr));
	ret = ft_rr(ptr, mp);
	fclose(ptr);
	free(mp);
	return (ret);
}

int		main(int ac, char **av)
{
	int ret = 1;
	if (ac != 2)
		ft_putstr("Error: argument\n");
	else
		ret = open_file(av[1]);
	return (ret);
}