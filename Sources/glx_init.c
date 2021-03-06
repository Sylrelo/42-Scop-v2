/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 00:04:12 by slopez            #+#    #+#             */
/*   Updated: 2021/06/21 18:40:43 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>

void			init_window(GLFWwindow **window, uint32_t width, uint32_t height)
{
	if (!glfwInit())
		die("glfwInit failed.");
	printf("[OpenGL] Initializing GLFW\n");

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
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

	///glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void			get_total_buffer_size(t_scop *scop, size_t *buffer_size, size_t *total_mats)
{
	size_t	i           = 0;
	size_t	j           = 0;

	*buffer_size = 0;
	if (total_mats)
		*total_mats = 0;

	while (i < scop->objects_count)
	{
		j = 0;
		while (j < scop->objects[i].nb_mats)
		{
			*buffer_size += scop->objects[i].materials[j].gl_buffer_size;
			j++;
		}
		if (total_mats)
			*total_mats += scop->objects[i].nb_mats;
		i++;
	}
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
			free(scop->objects[i].materials[j].gl_buffer);
			scop->objects[i].materials[j].gl_buffer = NULL;
			buffer_size += mat.gl_buffer_size;
			j++;
		}
		obj->offset += buffer_size;
		i++;
	}
}

void			init_opengl_buffer_multi(t_scop *scop)
{
	size_t buffer_size		= 0;
	size_t total_mats 		= 0;
	float *tmp_buffer 		= NULL;
	size_t total_triangles 	= 0;

	get_total_buffer_size(scop, &buffer_size, &total_mats);
	printf("[Scop] Merging all materials buffers\n");
	
	if (!(tmp_buffer = calloc(buffer_size, sizeof(float))))
		die ("Error calloc tmp_buffer");
	_floatset(tmp_buffer, 0.0f, buffer_size);

	merge_all_buffers(scop, &tmp_buffer);

	printf("[OpenGL] Binding bufffer and attribPointer\n");
	glGenBuffers(1, &scop->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, scop->vbo);

	glGenVertexArrays(1, &scop->vao);  
	glBindVertexArray(scop->vao);

	glBufferData(GL_ARRAY_BUFFER, (buffer_size * sizeof(float)), (void *) tmp_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, BUFFER_COMPONENT * sizeof(float), (const void *)(8 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

	free(tmp_buffer);
	tmp_buffer = NULL;

	printf("+ Total materials   : %zu\n", total_mats);
	total_triangles = (buffer_size / BUFFER_COMPONENT) / 3;

	if (total_triangles > 1000000)
		printf("+ Total triangles   : %.2fm (%zu)\n", total_triangles * 0.000001, total_triangles);
	else if (total_triangles > 1000)
		printf("+ Total triangles   : %.2fk (%zu)\n", total_triangles * 0.001, total_triangles);
	else
		printf("+ Total triangles   : %lu\n", total_triangles);
}