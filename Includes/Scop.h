/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 14:54:20 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

#include <stdlib.h>
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#include "Libmatvec.h"
#include "libc_extends.h"

// Do not edit theses values
#define MAT_GL_BUFFER_REALLOC_VALUE 5000
#define BUFFER_COMPONENT 11

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
	uint32_t	gl_texture;
	uint32_t	width;
	uint32_t	height;
}				t_textures;

typedef struct	s_mat
{
	t_vec3f		ka;
	t_vec3f		kd;
	t_vec3f		ks;
	t_vec3f		tf;
	float		*gl_buffer;
	int			tex_id;
	char		material_name[256];
	size_t		gl_buffer_size;
	size_t		tmp_allocated;
}				t_mat;

typedef struct	s_objects
{
	t_mat			*materials;
	size_t			nb_mats;
	t_vec3f			pos;
	t_vec3f			rot;
	t_vec3f			center;
	t_vec3f			max;
	t_vec3f			min;
	size_t			offset;
}				t_objects;

typedef struct	s_uniforms
{
	uint32_t	m4_projection;
	uint32_t	m4_view;
	uint32_t	m4_model;

	uint32_t	ka;
	uint32_t	kd;
	uint32_t	textured;

	uint32_t	glfw_time;
	uint32_t	glfw_options;

	uint32_t 	tex_size;
	uint32_t 	obj_max_y;
	uint32_t 	obj_center;

	uint32_t	tex_basic;
	uint32_t	tex_object;
}				t_uniforms;

typedef struct	s_ogl
{
	GLint		p_render;
	t_uniforms	u_render;
	GLint 		vao;
	int			s_texturing;
}				t_ogl;

typedef struct s_scop
{
	t_ogl			ogl;
	GLFWwindow		*window;
	GLuint			vao;
	GLuint			vbo;
	GLint			program;
	
	uint32_t		width;
	uint32_t		height;
	uint32_t		keys[349];

	t_textures		*textures;
	size_t			textures_count;

	t_objects		*objects;
	size_t			objects_count;

	float			multiplier;
	t_vec3f			cam_pos;
	t_vec3f			cam_rot;
}				t_scop;


// parse_shader.c
int         create_shader_program(char *file_vertex, char *file_fragment, char *file_geometry);

// scop_utils.c
void		die(char *string);
void   		print_matlist(size_t nb_mat, t_mat *materials);
void    	clean_exit(t_scop *scop);
float		random_float(int min, int max, float prec);


// glfw_events.c
void		handle_keyboard(GLFWwindow *window, uint32_t keys[349], int *s_texturing);
void 		handle_transformations(t_scop *scop);
void		handle_mouse(GLFWwindow *window, t_vec3f *cam_rot);

// glx_init.c
void		init_window(GLFWwindow **window, uint32_t width, uint32_t height);
void		init_opengl_buffer(t_scop *scop);
void		init_opengl_buffer_multi(t_scop *scop);
void		init_depth_map(t_scop *scop);


// parse_fdf.c
void 		parser_init_fdf(t_scop *scop, char *file);

// parse_face.c
size_t      parse_face(t_parser *parser, t_mat **materials, size_t *material_count, char *last_mtl, char *line);

// parsing_init.c
void        parser_init(t_scop *scop, char *argv);

// parsing_mtl.c
int        	parser_mtl_start(t_scop *scop, char path[256], char *file);
void        init_mat_default_values(t_mat *material);

// parsing_texture.c
void        parse_texture(t_scop *scop, t_mat *material, char path[256], char *file);
void        generate_gl_texture(t_scop *scop, size_t width, size_t height, void *tex, char *file, unsigned int type);

#endif