#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_rr
{
	float x, y, width, height;
	char type, c;
}				t_rr;

typedef struct s_mp
{
	int width, height;
	char bgc;
}				t_mp;

int		ft_strlen(char *s);
void	ft_putstr(char *s);
int		ft_err(void);
int		open_file(char *s);