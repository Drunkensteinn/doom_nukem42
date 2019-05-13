/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablizniu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 13:16:46 by yalytvyn          #+#    #+#             */
/*   Updated: 2019/05/10 14:29:19 by ablizniu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

#include <SDL.h>
#include "errors.h"
#include <math.h>
#include <unistd.h>
#include "libft.h"

#define RAY_BETWEEN_ANGLE 60.0 / (double)WIDTH

#define WIDTH 900
#define HEIGHT 600
#define	HALFWIDTH (double)WIDTH / 2.0
#define	FOV 60
#define	FPS 60
#define	TICKS_FRAME 333 / FPS
#define	VECX 1
#define	VECY 0
#define DIST (double)WIDTH * 1.3	// рассчет дальности плоскости проекци. для фова 60
#define	ANG 180
#define	HE_P 1150 / m->player.p_he

#define DELIMITER '\t'

typedef	struct	s_valid
{
	unsigned char	data;
	struct s_valid	*next;
}				t_valid;

typedef	struct	s_angle
{
	double		hor;
	double		ver;
}				t_angle;

typedef struct s_vertex
{
	double		x;
	double		y;
	double		z;
	double 		*x_arr;
	size_t		x_arr_size;
}				t_vertex;

typedef	struct	s_sendray
{
	int32_t	num_sect;
	double	angle;
}				t_sendray;


typedef struct	s_ray
{
	double		angle; // угол каста луча
	double		vx;		// вектор каста луча
	double		vy;
	double		ancos;	//пре кос син
	double		ansin;
	double		addlen;
	size_t		old_num_sect;
	size_t		num_sect;	// номер стены
	size_t		wall_sect;
	int32_t		len_ray;	
	int32_t		wall_type;
	int32_t		w;
	size_t		ray_deep;
	double		camdist;	//фикс
	t_vertex	intersec;	// результат чека пересения
	t_vertex	ray_end;		//  кординаты второй точки луча 
	t_vertex	ray_start;	// кординаты первой точки после портала
	t_vertex	wall_start;
	t_vertex	wall_end;
}				t_ray;

typedef	struct	s_save_ray
{
	t_vertex	sv_ray_end;
	t_vertex	sv_pos;
	int32_t		sv_sect;
	int32_t		sv_old_sect;

}				t_save_ray;


typedef struct	s_fps
{
	double		time;
	double		oldtime;
	double		frametime;
	double		looktime;
	double		lookoldtime;
}				t_fps;

typedef	struct	s_sdl
{
	SDL_Window	*window;
	SDL_Surface *winsurface;
	SDL_Surface	*skybox;
	SDL_Event	event;

}				t_sdl;

typedef struct	s_trplayer
{
	t_angle		angle;	// угол повота по осям
	t_vertex	pos;	// позиция игрока
	t_vertex	vec;	// вектор направления игрока
	int32_t		sector;	// номер сектора где находиться ирок
	int32_t		p_he;	// высота игрока
	float		movespeed;	//скорость
	float		rotspeed;
	int32_t		ecvator;

}               t_trpalyer;

typedef	struct	s_event
{
	int32_t		move_up;
	int32_t		move_down;
	int32_t		rot_rigth;
	int32_t		rot_left;
	int32_t		rot_up;
	int32_t		rot_down;
	int32_t		strafe_left;
	int32_t		strafe_rigth;
	int32_t		shoot_event;
	int32_t		jump_event;
}				t_event;

typedef	struct	s_heigth
{
	double		floor;
	double		cell;
}				t_heigth;


typedef struct	s_sky
{
	t_vertex	start;
	t_vertex	end;
	int32_t		h;
	int32_t		w;
	SDL_Surface	*sky;
}				t_sky;

typedef	struct	s_trans
{
	double		*transit_start;
	double		*transit_end;
	double		is_transition;
}				t_trans;

typedef	struct	s_sector
{
	size_t		color;
	size_t		sector_index;	// номер сектора	
	double		**vertex;
	size_t		vertex_arr_len;
	t_heigth	heigth;
	int32_t		*transit;		// массив точек сектора
}				t_sector;

typedef	struct	s_intersection
{
	t_vertex	dir1;
	t_vertex	dir2;
	double		a1;
	double		b1;
	double		d1;
	double		a2;
	double		b2;
	double		d2;
	double		seg_line1_s;
	double		seg_line1_e;
	double		seg_line2_s;
	double		seg_line2_e;
	double		u;
}				t_intersection;

typedef	struct	s_main
{
	t_vertex	*vertex; // массив структур ввсех точек карты
	t_sector	*sector; // массив структур всех секторов
	size_t		sum_sect;	// количество секторов
	size_t		sum_vert;	// количество вертексов
	size_t		sum_vert_pair;	// количество вертексов
	t_ray		ray;	// немного переменных
	t_sky		sky;
	t_fps		fps;	// фпс
	t_sdl		sdl;	// все сдл переменные
	t_trpalyer	player;	// данные о игроке
	t_event		eventcall; // обработчик движений и т.д.

}				t_main;

void			parse_player(t_main *main, t_list *list);

t_list			*find_elem(t_list *list, char *elem);

t_sector		processing(t_main *main, size_t index, char *sector_data);

size_t			content_len(t_list *list, char *content);

void			parse_sector(t_main *main, t_list *list);

void 			read_file(t_main *main);

t_main 			*init_main(void);

void			print_error(const char *msg);

void			two_dim_del(char ***array);

size_t			two_dim_len(char **array);

void			ft_sdlinit(t_main *m);
void			ft_sdlloop(t_main *m);
void			ft_init(t_main *m);
void			ft_key(t_main *m, int32_t *run);
void			ft_mouse(t_main *m);
void			ft_transform(t_main	*m);
void			ft_put_pixel(t_main *m, int32_t x, int32_t y, int32_t pixel);
int				ft_get_pixel(SDL_Surface *texture, int32_t x, int32_t y);
void			ft_fps_utils(t_main *m);
void			ft_fps_look(t_main *m);
void			ft_draw_map(t_main *m);
void			drawline(t_main *m, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
t_vertex		ft_intersection(t_vertex st1, t_vertex end1, t_vertex st2, t_vertex end2);
void			ft_drawscreen(t_main *m, t_ray ray);
void			ft_draw_floor(t_main *m, int32_t start, int32_t end, int32_t x);
void			ft_load_texture(t_main *m);
void			ft_ray(t_main *m, t_ray ray);
int				ft_collision(t_main *m, t_vertex start, t_vertex end);
int				ft_cmp_vertex(t_vertex one, t_vertex two);
void			ft_init_sky(t_main *m);
void			ft_draw_sky(t_main *m);

#endif