/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 15:58:44 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include "Prototypes.Parsing.h"
#include <stdio.h>

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

	*window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
	printf("[OpenGL] Creating window\n");
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
	while (!glfwWindowShouldClose(scop->window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(scop->window);
		glfwPollEvents();
	}
}

int main(int argc, char *argv[])
{
	t_scop	scop;

	printf("[Scop] Starting parser\n");
	parser_init(&scop, argv[1]);

	printf("- Triangles count : %zu\n", scop.nb_triangles);
	printf("- Materials count : %zu\n\n", scop.nb_mats);

	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop.window);
	//


	glGenBuffers(1, &scop.vbo);  
	glBindBuffer(GL_ARRAY_BUFFER, scop.vbo);

	glGenVertexArrays(1, &scop.vao);  
	glBindVertexArray(scop.vao);

	printf("%zu\n", (sizeof(float) * 8));

	//static const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	0.0f,  1.0f, 0.0f,
	//};

	//glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, ((uint64_t)scop.nb_triangles) * (8 * sizeof(float)), NULL, GL_DYNAMIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(3 * sizeof(float)));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(6 * sizeof(float)));

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);


	scop.program = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl");
	glUseProgram(scop.program);
	//

	printf("[Scop] Ready\n");
	display_loop(&scop);

	(void)argv;
	(void)argc;
}