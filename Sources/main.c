/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/14 23:20:04 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <math.h>

void	display_loop(t_scop *scop)
{
	t_mat	material;
	size_t	mat_i 	= 0;
	size_t	offset 	= 0;

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

	float a = 0;

	// glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glUniform3f(loc_lightpos, 0, 0, 0);
	glUniform3f(loc_lightcol, 1, 1, 1);

	while (!glfwWindowShouldClose(scop->window))
	{
		mat_i 	= 0;
		offset 	= 0;
		a 		+= 0.010;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		handle_keyboard(scop->window, scop->keys);
		handle_mouse(scop->window,  &scop->cam_rot);
		handle_transformations(scop->keys, &scop->cam_pos, &scop->cam_rot);

		mat_view = m4_viewmat(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z, 
							m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));

		mat_model = m4_mult(
						m4_mult(
							m4_scale(1, 1, 1), 
							m4_rotation_around_center(scop->center, 0, 0, 0)), 
						m4_translate(0, 0, 0)
					);

		glUniformMatrix4fv(glMatView, 1, GL_FALSE, mat_view.value[0]);
		glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat_model.value[0]);
		
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

			glDrawArrays(GL_TRIANGLES, offset / 8, material.gl_buffer_size / 8);
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

	scop->cam_pos = (t_vec3f) {0, 0, 0};
	scop->cam_rot = (t_vec3f) {0, 0, 0};
	
	scop->width = 1280;
	scop->height = 720;

	memset(scop->keys, 0, sizeof(uint32_t) * 348);
		
	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);



	// test
	//tester existence des fichiers avant calloc
	scop->objects_count = argc - 1;
	scop->objects = calloc(scop->objects_count, sizeof(t_objects));

	//fin test
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

	scop->cam_pos.x -= scop->center.x * .5;
	scop->cam_pos.y -= scop->center.y * .5;
	scop->cam_pos.z -= scop->center.z + 8;

	display_loop(scop);

	clean_exit(scop);
	(void)argc;
}