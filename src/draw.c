/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalytvyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 15:36:20 by yalytvyn          #+#    #+#             */
/*   Updated: 2019/03/22 15:36:22 by yalytvyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	ft_put_pixel(t_main *m, int x, int y, int32_t pixel)
{
	int		*target_pixel;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT - 1)
		return ;
	target_pixel = m->sdl.winsurface->pixels + y * m->sdl.winsurface->pitch + x *
	m->sdl.winsurface->format->BytesPerPixel;
	*target_pixel = pixel;
}

void	ft_put_pixel_rgb(t_main *m, int x, int y, t_rgb col)
{
	int32_t	*target_pixel;
	int32_t	color;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT - 1)
		return ;
	target_pixel = m->sdl.winsurface->pixels + y * m->sdl.winsurface->pitch + x *
	m->sdl.winsurface->format->BytesPerPixel;
	color = SDL_MapRGB(m->texture[0]->format, col.r, col.g, col.b);
	*target_pixel = color;
	//*target_pixel = pixel;
}
//берем пиксель с текстуры / потом


// брезенхем
void	drawline(t_main *m, int x1, int y1, int x2, int y2)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;

	int error = deltaX - deltaY;
	ft_put_pixel(m, x2, y2, 0xff00ff);
	while(x1 != x2 || y1 != y2) 
   {
	   ft_put_pixel(m, x1, y1, 0xff00ff);
		const int error2 = error * 2;
		if(error2 > -deltaY) 
		{
			error -= deltaY;
			x1 += signX;
		}
		if(error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void	ft_get_wall_heigth(t_main *m, t_ray ray, t_heigth_wall *wall)
{
	wall->he_wall = DIST / ray.camdist;
	wall->he_sect = fabs(m->sector[ray.num_sect].heigth.cell - m->sector[ray.num_sect].heigth.floor);
	wall->diff = wall->he_wall / 10.0;
	wall->floor = m->sector[ray.num_sect].heigth.floor - m->player.pos.z;
	wall->ceil = m->sector[ray.num_sect].heigth.cell - m->player.pos.z;
	wall->end = m->player.ecvator + wall->diff * m->player.p_he - wall->diff * wall->floor * 2;
	wall->start = wall->end - wall->he_wall - wall->diff * wall->he_sect + wall->diff * wall->floor;
	wall->wall_h_map = abs(wall->end - wall->start);
}


void	ft_get_border_bot(t_main *m, t_ray ray, t_heigth_wall *wall)
{
	if (ray.next_sect == -1)
	{
			wall->border_bot.flag = 0;
			return ;
	}
	if (m->sector[ray.num_sect].heigth.floor < m->sector[ray.next_sect].heigth.floor)
	{
		wall->diff_bord_bot = m->sector[ray.next_sect].heigth.floor - m->sector[ray.num_sect].heigth.floor;
		wall->border_bot.end = wall->end;
		wall->border_bot.start = wall->border_bot.end - wall->diff * wall->diff_bord_bot * 2;
		wall->border_bot.flag = 1;
		wall->border_b_h = wall->border_bot.end - wall->border_bot.start;
	}
	else
		wall->border_bot.flag = 0;
}

void	ft_get_floor(t_main *m, t_ray ray, t_heigth_wall *wall, t_buffer *buf)
{
	ray.camdist = ray.camdist;
	m->player.sector = m->player.sector;
	if (wall->border_bot.flag == 0)
		wall->floor_h.start = wall->end;
	else
		wall->floor_h.start = wall->border_bot.end;
	wall->floor_h.end = buf->buffer_draw_bot;
	if (wall->floor_h.start < wall->floor_h.end)
		wall->floor_h.flag = 1;
	else
		wall->floor_h.flag = 0;
	wall->floor_h.floor_h = wall->floor_h.end - wall->floor_h.start;
}

void	ft_get_border_top(t_main *m, t_ray ray, t_heigth_wall *wall)
{
	if (ray.next_sect == -1)
	{
			wall->border_top.flag = 0;
			return ;
	}
	if (m->sector[ray.num_sect].heigth.cell > m->sector[ray.next_sect].heigth.cell)
	{
		wall->diff_bord_top = m->sector[ray.num_sect].heigth.cell - m->sector[ray.next_sect].heigth.cell;
		wall->border_top.start = wall->start;
		wall->border_top.end = wall->border_top.start + wall->diff * wall->diff_bord_top;
		wall->border_t_h = wall->border_top.end - wall->border_top.start;
		wall->border_top.flag = 1;
		if (ray.w == HALFWIDTH)
		{
			m->debug.top_start = wall->border_bot.start;
			m->debug.top_end = wall->border_bot.end;
		}
	}
	else
		wall->border_top.flag = 0;
}

void	ft_get_ceil(t_main *m, t_ray ray, t_heigth_wall *wall, t_buffer *buf)
{
	m->player.sector = m->player.sector;
	ray.camdist = ray.camdist;
	if (wall->border_top.flag == 0)
		wall->ceil_h.end = wall->start;
	else
		wall->ceil_h.end = wall->border_top.start;
	wall->ceil_h.start = buf->buffer_draw_top;
	if (wall->ceil_h.start < wall->ceil_h.end)
		wall->ceil_h.flag = 1;
	else
		wall->ceil_h.flag = 0;
}

inline void	ft_drawscreen(t_main *m, t_ray ray)
{
	t_heigth_wall		wall;
	t_buffer			buffer;
	static int32_t	border_b_h = 0;
	static int32_t	border_t_h = 0;
	static t_draw_save	sv_draw = {0, HEIGHT, 0};	


	wall.border_b_h = 0;
	wall.border_t_h = 0;
	if (sv_draw.old_w != ray.w)
	{
		sv_draw.buffer_bot = HEIGHT;
		sv_draw.buffer_top = 0;
	}
	wall.dx = ray.wall_end.x - ray.wall_start.x;
	wall.dy = ray.wall_end.y - ray.wall_start.y;
	wall.y = (fabs(wall.dx) > fabs(wall.dy) ? (int)((ray.intersec.x - ray.wall_start.x) * 1000) :
							(int)((ray.intersec.y - ray.wall_start.y) * 1000)) % m->texture[0]->w;
	wall.old_border_b_h = border_b_h;
	wall.old_border_t_h = border_t_h;
	if (ray.camdist < 0.005)
		ray.camdist = 0.005;
	buffer.buffer_draw_bot = sv_draw.buffer_bot;
	buffer.buffer_draw_top = sv_draw.buffer_top;
	ft_get_wall_heigth(m, ray, &wall);
	ft_get_border_bot(m, ray, &wall);
	ft_get_border_top(m, ray, &wall);
	ft_get_floor(m, ray, &wall, &buffer);
	ft_get_ceil(m, ray, &wall, &buffer);
	if (wall.floor_h.flag == 1)
		ft_draw_floor(m, ray, wall, &buffer);
	if (m->sector[ray.num_sect].transit[ray.wall_sect] == -1)
		ft_draw_wall(m, ray, wall, &buffer);
	if (wall.border_bot.flag == 1 || wall.border_top.flag == 1)
		ft_draw_border(m, ray, wall, &buffer);
	if (wall.ceil_h.flag == 1)
		ft_draw_cell(m, ray, wall, &buffer);
	border_b_h = wall.border_b_h;
	border_t_h = wall.border_t_h;

	sv_draw.buffer_bot = wall.end < buffer.buffer_draw_bot ? wall.end : buffer.buffer_draw_bot;
	sv_draw.buffer_top = wall.start > buffer.buffer_draw_top ? wall.start : buffer.buffer_draw_top;
	sv_draw.old_w = ray.w;
}