/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 00:07:42 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <math.h>

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
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

	t_mat4	mat_perspective = m4_perspective(1.0472, (float) scop->width / (float) scop->height, 0.00001f, 1000.0f);
	t_mat4	mat_view 		= m4_init();
	t_mat4	mat_model 		= m4_init();

	glUniformMatrix4fv(glMatPersp, 1, GL_FALSE, mat_perspective.value[0]);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);  
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	float a = 0;

	size_t	mat_i = 0;
	size_t	offset = 0;
	
	while (!glfwWindowShouldClose(scop->window))
	{
		glfwHandleKeysPress(scop->window, &scop->keys);
		handle_mouse(scop->window,  &scop->camera_rotation);
		handle_transformation(scop->keys, &scop->camera_position, &scop->camera_rotation);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		a += 0.010;

		mat_view = m4_viewmat(scop->camera_rotation.x, scop->camera_rotation.y, scop->camera_rotation.z, 
							m4_translate(scop->camera_position.x, scop->camera_position.y, scop->camera_position.z) );

		mat_model = (m4_mult(
			m4_mult( m4_scale(1, 1, 1), m4_rotation_around_center(scop->center, 0, cos(a * 2), 0)), 
			m4_translate(0, 0, 0))
			);


		// m4_print(m4_rotation(scop->camera_rotation.x, scop->camera_rotation.y, scop->camera_rotation.z));

		glUniformMatrix4fv(glMatView, 1, GL_FALSE, mat_view.value[0]);
		glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat_model.value[0]);

		glUniform3f(loc_lightpos, 0, 0, 0);
		glUniform3f(loc_lightcol, 1, 1, 1);
		
		mat_i 	= 0;
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

int 	main(int argc, char *argv[])
{
	t_scop	*scop;

	scop = calloc(1, sizeof(t_scop));

	scop->nb_mats = 0;
	scop->textures_count = 0;
	scop->textures = NULL;

	scop->keys = 0x0;
	scop->camera_position = (t_vec3f) {0, 0, 0};
	scop->camera_rotation = (t_vec3f) {0, 0, 0};
	
	scop->width = 1280;
	scop->height = 720;

	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);

	printf("[Scop] Starting parser\n");
	parser_init(scop, argv[1]);

	printf("- Materials count : %zu\n\n", scop->nb_mats);
	
	printf("[Scop] Starting OpenGL Buffer initialization\n");
	init_opengl_buffer(scop);
	printf("[Scop] Opening Window\n");
	glfwShowWindow(scop->window);

	scop->program = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl");
	glUseProgram(scop->program);

	printf("[Scop] Ready\n");

	scop->camera_position.x -= scop->center.x * .5;
	scop->camera_position.y -= scop->center.y * .5;
	scop->camera_position.z -= scop->center.z + 8;

	display_loop(scop);

	if(scop->nb_mats)
		free(scop->materials);
	(void)argc;
}