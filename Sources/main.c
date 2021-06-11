/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 16:25:09 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include "Prototypes.Parsing.h"
#include <stdio.h>
#include "matrices.h"

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
}

void	init_window(GLFWwindow **window)
{
	if (!glfwInit())
		die("glfwInit failed.");
	printf("[OpenGL] Initializing GLFW\n");

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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
	glfwSwapInterval(1);
}

void	display_loop(t_scop *scop)
{
	t_mat	material;

	GLuint glMatView 		= glGetUniformLocation(scop->program, "View");
	GLuint glMatPersp 		= glGetUniformLocation(scop->program, "Persp");
	GLuint glMatModel 		= glGetUniformLocation(scop->program, "Model");

	uint32_t loc_kd			= glGetUniformLocation(scop->program, "kd");
	uint32_t loc_ka			= glGetUniformLocation(scop->program, "ka");
	uint32_t loc_textured 	= glGetUniformLocation(scop->program, "textured");
	uint32_t loc_lightpos 	= glGetUniformLocation(scop->program, "lightPos");
	uint32_t loc_lightcol 	= glGetUniformLocation(scop->program, "lightColor");

	t_mat4	mat_perspective = m4_perspective(1.0472, 1280.0f / 720.0f, 0.00001f, 1000.0f);
	t_mat4	mat_view;
	t_mat4	mat_model;

	glUniformMatrix4fv(glMatPersp, 1, GL_FALSE, mat_perspective.value[0]);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);  
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	float a = 0;

	size_t	mat_i = 0;
	size_t	offset = 0;

	while (!glfwWindowShouldClose(scop->window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		a += 0.010;
		
		mat_view 	= m4_mult3(
			m4_rotation(0, 0, 0), 
			m4_scale(1, 1, 1), 
			m4_translate(0, 0, -6)
			);

		mat_model 	= m4_mult3(
			m4_rotation_around_center(scop->center, 0, a / 2, 0), 
			m4_scale(1, 1, 1), 
			m4_translate(0, 0, -3)
			);

		glUniformMatrix4fv(glMatView, 1, GL_FALSE, mat_view.value[0]);
		glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat_model.value[0]);

		glUniform3f(loc_lightpos, 0, 0, 0);
		glUniform3f(loc_lightcol, 1, 1, 1);
		
\		mat_i 	= 0;
		offset 	= 0;
		while (mat_i < scop->nb_mats)
		{
			material = scop->materials[mat_i];
			if (mat_i > 0)
				offset += scop->materials[mat_i - 1].gl_buffer_size;
			glUniform3f(loc_kd, material.kd.x, material.kd.y, material.kd.z);
			glUniform3f(loc_ka, material.ka.x, material.ka.y, material.ka.z);

			if (material.tex_id != -1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, scop->textures[material.tex_id].gl_texture);
				glUniform1i(loc_textured, 1);
			}
			else
				glUniform1i(loc_textured, 0);

			// printf("%d %d\n", mat_i, material.tex_id);

			glDrawArrays(GL_TRIANGLES, (offset / 8), (material.gl_buffer_size / 8));
			mat_i++;
		}
		glfwSwapBuffers(scop->window);
		glfwPollEvents();
	}
}

void	init_opengl_buffer(t_scop *scop)
{
	size_t	i = 0;
	size_t	buffer_size = 0;
	float	*tmp_buffer;

	glGenBuffers(1, &scop->vbo);  
	glBindBuffer(GL_ARRAY_BUFFER, scop->vbo);

	glGenVertexArrays(1, &scop->vao);  
	glBindVertexArray(scop->vao);

	printf("[Scop] Merging all materials buffers\n");
	while (i < scop->nb_mats)
	{
		buffer_size += scop->materials[i].gl_buffer_size;
		i++;
	}

	if (!(tmp_buffer = calloc(buffer_size, sizeof(float))))
		die ("Error calloc tmp_buffer");
	_floatset(tmp_buffer, 0.0f, buffer_size);

	i = 0;
	buffer_size = 0;
	while (i < scop->nb_mats)
	{
		_floatncat(tmp_buffer, scop->materials[i].gl_buffer, buffer_size, scop->materials[i].gl_buffer_size);
		buffer_size += scop->materials[i].gl_buffer_size;
		i++;
	}

	printf("[OpenGL] Binding bufffer and attribPointer\n");
	glBufferData(GL_ARRAY_BUFFER, (buffer_size * sizeof(float)), (void *) tmp_buffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	free(tmp_buffer);
}

int 	main(int argc, char *argv[])
{
	t_scop	scop;

	scop.nb_mats = 0;
	scop.nb_triangles = 0;
	scop.textures_count = 0;
	scop.textures = NULL;

	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop.window);

	printf("[Scop] Starting parser\n");
	parser_init(&scop, argv[1]);

	printf("- Triangles count : %zu\n", scop.nb_triangles);
	printf("- Materials count : %zu\n\n", scop.nb_mats);
	
	
	printf("[Scop] Starting OpenGL Buffer initialization\n");
	init_opengl_buffer(&scop);
	printf("[Scop] Opening Window\n");
	glfwShowWindow(scop.window);

	scop.program = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl");
	glUseProgram(scop.program);

	printf("[Scop] Ready\n");
	display_loop(&scop);

	if(scop.nb_mats)
		free(scop.materials);
	(void)argc;
}