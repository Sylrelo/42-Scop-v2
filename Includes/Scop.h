/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/11 14:43:07 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

#include <stdlib.h>
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#include "Libmatvec.h"
#include "libc_extends.h"

#define MAT_GL_BUFFER_REALLOC_VALUE 5000

typedef enum e_parsing
{
	VERTICE	= 0x01,
	TEXTURE = 0x02,
	NORMAL	= 0x04,
}			e_parsing;

typedef struct s_parser
{
	t_vec3f		min;
	t_vec3f		max;
	t_vec3f		*v;
	t_vec3f		*vn;
	t_vec2f		*vt;
	size_t		v_count;
	size_t		vn_count;
	size_t		vt_count;
}				t_parser;

typedef struct	s_textures
{
	char		filename[256];
	float		*content;
	uint32_t	gl_texture;
}				t_textures;

typedef struct	s_mat
{
	char		material_name[256];
	int			tex_id;
	t_vec3f		ka;
	t_vec3f		kd;
	t_vec3f		ks;
	t_vec3f		tf;
	float		*gl_buffer;
	size_t		gl_buffer_size;
	size_t		tmp_allocated;
}				t_mat;

typedef enum	s_actions
{
	NONE		= 0x00,
	FORWARD 	= 0x01,
	BACKWARD 	= 0x02,
	LEFT 		= 0x04,
	RIGHT 		= 0x08,
}				t_actions;

typedef struct s_scop
{
	GLFWwindow		*window;
	GLuint			vao;
	GLuint			vbo;
	GLint			program;
	t_vec3f			center;

	uint32_t		width;
	uint32_t		height;
	
	uint32_t		keys;

	t_textures		*textures;
	size_t			textures_count;

	size_t			nb_triangles;
	size_t			nb_mats;
	t_mat			*materials;

	t_vec3f			camera_position;
	t_vec3f			camera_rotation;
}				t_scop;


void		die(char *string);
int    		create_shader_program(char *file_vertex, char *file_fragment);
void   		print_matlist(size_t nb_mat, t_mat *materials);

//
t_vec3f		vec_add(t_vec3f u, t_vec3f v);
t_vec3f		vec_sub(t_vec3f u, t_vec3f v);
t_vec3f		vec_cross(t_vec3f u, t_vec3f v);
t_vec3f		vec_multf(t_vec3f u, float f);
float		vec_length(t_vec3f u);
t_vec3f		vec_normalize(t_vec3f u);

//

#endif