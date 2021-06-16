/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/16 23:04:12 by slopez           ###   ########.fr       */
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
	uint32_t	gl_texture;
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

typedef struct	s_ogl
{
	GLint		p_ddepthmap;
	GLint		p_render;

	GLuint		depth_map_fbo;
	GLuint		depth_map;
}				t_ogl;

typedef struct s_scop
{
	// OpenGL & GLFW
	t_ogl			ogl;
	GLFWwindow		*window;
	GLuint			vao;
	GLuint			vbo;
	GLint			program;
	
	t_vec3f			center;

	uint32_t		width;
	uint32_t		height;
	uint32_t		keys[349];

	t_textures		*textures;
	size_t			textures_count;

	t_objects		*objects;
	size_t			objects_count;

	t_vec3f			cam_pos;
	t_vec3f			cam_rot;
}				t_scop;


// parse_shader.c
int         create_shader_program(char *file_vertex, char *file_fragment, char *file_geometry);

// scop_utils.c
void		die(char *string);
void   		print_matlist(size_t nb_mat, t_mat *materials);
void    	clean_exit(t_scop *scop);


// glfw_events.c
void		handle_keyboard(GLFWwindow *window, uint32_t keys[349]);
void 		handle_transformations(uint32_t keys[349], t_vec3f *cam_pos, t_vec3f *cam_rot);
void		handle_mouse(GLFWwindow *window, t_vec3f *cam_rot);

// glx_init.c
void		init_window(GLFWwindow **window, uint32_t width, uint32_t height);
void		init_opengl_buffer(t_scop *scop);
void		init_opengl_buffer_multi(t_scop *scop);
void		init_depth_map(t_scop *scop);


// parse_fdf.c
void 		parser_init_fdf(t_scop *scop, char *file);

// parse_face.c
void        mat_push_buffer(t_mat *material, float *buffer, size_t buffer_size);
size_t      parse_face(t_parser *parser, t_mat **materials, size_t *material_count, char *last_mtl, char *line);
void        update_minmax(t_vec3f *min, t_vec3f *max, float buffer[8]);
void        calculate_missing_normal(t_mat *material);

// parsing_init.c
void        parser_init(t_scop *scop, char *argv);

// parsing_mtl.c
int        	parser_mtl_start(t_scop *scop, char path[256], char *file);
void        init_mat_default_values(t_mat *material);

// parsing_texture.c
void        parse_texture(t_scop *scop, t_mat *material, char path[256], char *file);

#endif