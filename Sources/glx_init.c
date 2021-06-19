/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 00:04:12 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 13:49:42 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>

void			init_window(GLFWwindow **window, uint32_t width, uint32_t height)
{
	if (!glfwInit())
		die("glfwInit failed.");
	printf("[OpenGL] Initializing GLFW\n");

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_SAMPLES, 4);

	*window = glfwCreateWindow(width, height, "Scop", NULL, NULL);
	glfwHideWindow(*window);
	printf("[OpenGL] Creating window and context\n");
	if (!window)
	{
		glfwTerminate();
		die("glfwCreateWindow failed.");
	}
	glfwMakeContextCurrent(*window);
	if (gl3wInit())
	{
		glfwTerminate();
		die("gl3wInit failed.");
	}
	printf("[OpenGL] Initializing GL3W\n");
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

#include <math.h>
static void	get_total_buffer_size(t_scop *scop, size_t *buffer_size, size_t *total_mats)
{
	size_t	i           = 0;
	size_t	j           = 0;

	t_vec3f point;
	int		offset = 0 * 8;
	t_vec3f max;
	t_vec3f center;

	while (i < scop->objects_count)
	{
		max = scop->objects[i].max;
		center = scop->objects[i].center;
		
		j = 0;
		while (j < scop->objects[i].nb_mats)
		{

			point.x = scop->objects[i].materials[j].gl_buffer[0 + offset];
			point.y = scop->objects[i].materials[j].gl_buffer[1 + offset];
			point.z = scop->objects[i].materials[j].gl_buffer[2 + offset];
			
			*buffer_size += scop->objects[i].materials[j].gl_buffer_size;
			j++;
		}
		*total_mats += scop->objects[i].nb_mats;
		i++;
	}
	
	float u, v;

	printf("\n");

	//point.z *= 1;
	// point.x *= -1;
	
	center.x = 0;
	center.y = 0;
	center.z = 0;

	point.x = 0;
	point.y = 0;
	point.z = 0;

	u = 0.5 + atan2f(point.z - center.z, point.x - center.x) / (M_PI * 2.0f);

	v = point.y / max.y;

	printf("u = .5 + atan2f(%f - %f, %f - %f) / (M_PI * 2.0f)\n", point.z, center.z, point.x, center.x);
	printf("atan2f : atan2f(%f, %f) = %f\n", point.z - center.z, point.x - center.x, atan2f(point.z - center.z, point.x - center.x));

	printf("v = %f / %f\n", point.y, max.y);

	printf("u: %f\nv: %f\n\n", u, v);

	printf("atan2f (%d, %d) = %f\n", -42, -42, atan2f(-42, -42));
	printf("atan2f (%d, %d) = %f\n", -42, 42, atan2f(-42, 42));
	printf("atan2f (%d, %d) = %f\n", 42, -42, atan2f(42, -42));
	printf("atan2f (%d, %d) = %f\n", 42, 42, atan2f(42, 42));
}

static void		merge_all_buffers(t_scop *scop, float **buffer)
{
	size_t i 			= 0;
	size_t j 			= 0;
	size_t buffer_size 	= 0;
	t_objects *obj		= NULL;
	t_mat mat;

	i = 0;
	buffer_size = 0;
	while (i < scop->objects_count)
	{
		j = 0;
		obj = &scop->objects[i];
		obj->offset = 0;
		while (j < obj->nb_mats)
		{
			mat = obj->materials[j];
			_floatncat(*buffer, mat.gl_buffer, buffer_size, mat.gl_buffer_size);
			buffer_size += mat.gl_buffer_size;
			j++;
		}
		obj->offset += buffer_size;
		i++;
	}
}

static void		free_all_buffers(t_scop *scop, float **buffer)
{
	size_t i = 0;
	size_t j = 0;

	i = 0;
	while (i < scop->objects_count)
	{
		j = 0;
		while (j < scop->objects[i].nb_mats)
		{
			free(scop->objects[i].materials[j].gl_buffer);
			scop->objects[i].materials[j].gl_buffer = NULL;
			j++;
		}
		i++;
	}
	free(*buffer);
	*buffer = NULL;
}

void			init_opengl_buffer_multi(t_scop *scop)
{;
	size_t buffer_size 	= 0;
	size_t total_mats 	= 0;
	float *tmp_buffer 	= NULL;

	get_total_buffer_size(scop, &buffer_size, &total_mats);
	
	printf("[Scop] Merging all materials buffers\n");
	
	if (!(tmp_buffer = calloc(buffer_size, sizeof(float))))
		die ("Error calloc tmp_buffer");
	_floatset(tmp_buffer, 0.0f, buffer_size);

	merge_all_buffers(scop, &tmp_buffer);

	printf("[OpenGL] Binding bufffer and attribPointer\n");
	glGenBuffers(1, &scop->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, scop->vbo);
	printf("%d\n", glGetError());

	glGenVertexArrays(1, &scop->vao);  
	glBindVertexArray(scop->vao);
	printf("%d\n", glGetError());

	glBufferData(GL_ARRAY_BUFFER, (buffer_size * sizeof(float)), (void *) tmp_buffer, GL_STATIC_DRAW);
	printf("%d\n", glGetError());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(8 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
	printf("%d\n", glGetError());

	free_all_buffers(scop, &tmp_buffer);
	// printf("+ Total buffer size : %zu\n", buffer_size);
	printf("+ Total materials   : %zu\n", total_mats);
	printf("+ Total triangles   : %zu\n", (buffer_size / 8) / 3);
}