/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 16:48:08 by slopez           ###   ########lyon.fr   */
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
#define MAT_GL_BUFFER_REALLOC_VALUE 10000
#define BUFFER_COMPONENT 11

typedef enum e_parsing
{
	VERTICE	= 0x01,
	TEXTURE = 0x02,
	NORMAL	= 0x04,
}			e_parsing;

typedef struct s_parser
{
	size_t		v_count;
	size_t		vn_count;
	size_t		vt_count;
	t_vec3f		min;
	t_vec3f		max;
	t_vec3f		*v;
	t_vec3f		*vn;
	t_vec2f		*vt;
}				t_parser;

typedef struct	s_textures
{
	uint32_t	gl_texture;
	uint32_t	width;
	uint32_t	height;
	char		filename[128];
}				t_textures;

typedef struct	s_mat
{
	int			tex_id;
	size_t		gl_buffer_size;
	size_t		tmp_allocated;
	float		*gl_buffer;
	float		ns;
	t_vec3f		ka;
	t_vec3f		kd;
	t_vec3f		ks;
	t_vec3f		tf;
	char		material_name[64];
}				t_mat;

typedef struct	s_objects
{
	size_t			nb_mats;
	size_t			offset;
	float 			scale;
	t_vec3f			pos;
	t_vec3f			rot;
	t_vec3f			center;
	t_vec3f			max;
	t_vec3f			min;
	t_mat			*materials;
}				t_objects;

typedef struct	s_uniforms
{
	uint32_t	m4_projection;
	uint32_t	m4_view;
	uint32_t	m4_model;
	uint32_t	m4_light;

	uint32_t	ka;
	uint32_t	kd;
	uint32_t	ks;
	uint32_t	ns;
	uint32_t	textured;
	uint32_t	mapping;

	uint32_t	glfw_time;
	uint32_t	glfw_options;

	uint32_t 	tex_size;
	uint32_t 	obj_max_y;
	uint32_t 	obj_center;

	uint32_t	tex_basic;
	uint32_t	tex_object;
	uint32_t 	tex_shadowmap;
	uint32_t	object_selected;
	uint32_t	view_pos;

}				t_uniforms;

typedef struct	s_ogl
{
	int			s_texturing;
	int 		s_mapping;

	uint32_t 	fbo_depth;
	uint32_t 	tex_depth;	

	GLint		p_render;
	GLint		p_depth;

	GLint 		vao;
	t_uniforms	u_render;
}				t_ogl;

typedef struct s_scop
{
	uint32_t		keys[349];
	t_textures		*textures;
	t_objects		*objects;
	t_ogl			ogl;
	t_vec3f			cam_rot;
	GLFWwindow		*window;
	float 			fade_start_time;
	t_vec3f			cam_pos;
	float			multiplier;
	size_t			textures_count;
	size_t			objects_count;
	uint32_t		vao;
	uint32_t		vbo;
	uint32_t		width;
	uint32_t		height;
	int				program;
	int 			basic_texture_id;
	short 			selected_object;
}				t_scop;

// display.c
void		display_loop(t_scop *scop);


// parse_shader.c
int         create_shader_program(char *file_vertex, char *file_fragment, char *file_geometry);

// scop_utils.c
void		die(char *string);
void   		print_matlist(size_t nb_mat, t_mat *materials);
void    	clean_exit(t_scop *scop);
float		random_float(int min, int max, float prec);


// glfw_events.c
void        handle_keyboard(GLFWwindow *window, uint32_t keys[349], int *s_texturing, int *s_mapping, short *selected_object, size_t objects_count);
void 		handle_transformations(t_scop *scop);
void		handle_mouse(GLFWwindow *window, t_vec3f *cam_rot);

// glx_init.c
void		init_window(GLFWwindow **window, uint32_t width, uint32_t height);
void		init_opengl_buffer(t_scop *scop);
void		init_opengl_buffer_multi(t_scop *scop);
void		init_depth_map(t_scop *scop);
void		get_total_buffer_size(t_scop *scop, size_t *buffer_size, size_t *total_mats);


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