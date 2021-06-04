/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 14:30:54 by slopez           ###   ########lyon.fr   */
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

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, 1);
	glfwSwapInterval(1);
}

void	display_loop(t_scop *scop)
{
	while (!glfwWindowShouldClose(scop->window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(scop->window);
		glfwPollEvents();
	}
}

int main(int argc, char *argv[])
{
	t_scop	scop;

	parser_init(&scop, argv[1]);

	init_window(&scop.window);
	//

	glGenBuffers(1, &scop.vbo);  
	glBindBuffer(GL_ARRAY_BUFFER, scop.vbo);

	glGenVertexArrays(1, &scop.vao);  
	glBindVertexArray(scop.vao);

	//
	display_loop(&scop);

	(void)argv;
	(void)argc;
}