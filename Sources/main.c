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

	GLuint glMatTransform 	= glGetUniformLocation(scop->program, "Transform");
	GLuint glMatPersp 		= glGetUniformLocation(scop->program, "Persp");
	GLuint glMatModel 		= glGetUniformLocation(scop->program, "Model");

	mat4f 	mat_persp;
	mat4f 	mat_model;
	mat4f 	mat_transform;
	// mat4f	mat_scale;

	// mat4_init(mat_model);
	mat4_init(mat_transform);
	mat4_perspective(mat_persp, 1.0472, 1280.0f / 720.0f, 0.00001f, 1000.0f);

	glUniformMatrix4fv(glMatPersp, 1, GL_FALSE, mat_persp[0]);
	glUniformMatrix4fv(glMatTransform, 1, GL_FALSE, mat_transform[0]);



	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);  
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	float a = 0;

	size_t	mat_i = 0;
	size_t	offset = 0;


	// A déplacer
	uint32_t loc_kd = glGetUniformLocation(scop->program, "kd");

	//

	while (!glfwWindowShouldClose(scop->window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		a += 0.010;
		matmat(mat_model, (t_vec3f) {0, 0, -5}, (t_vec3f){0, a, 0}, 2);
    	glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat_model[0]);

		mat_i 	= 0;
		offset 	= 0;
		while (mat_i < scop->nb_mats)
		{
			material = scop->materials[mat_i];
			if (mat_i > 0)
				offset += scop->materials[mat_i - 1].gl_buffer_size;
			glUniform3f(loc_kd, material.kd.x, material.kd.y, material.kd.z);
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