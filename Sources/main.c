/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/11 18:28:50 by slopez           ###   ########lyon.fr   */
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

void	glfwHandleKeysPress(GLFWwindow *window, uint32_t *keys)
{

	if (glfwGetKey(window, GLFW_KEY_W))
		*keys |= FORWARD;
	else if (*keys & FORWARD)
		*keys ^= FORWARD;

	if (glfwGetKey(window, GLFW_KEY_S))
		*keys |= BACKWARD;
	else if (*keys & BACKWARD)
		*keys ^= BACKWARD;

	if (glfwGetKey(window, GLFW_KEY_A))
		*keys |= LEFT;
	else if (*keys & LEFT)
		*keys ^= LEFT;

	if (glfwGetKey(window, GLFW_KEY_D))
		*keys |= RIGHT;
	else if (*keys & RIGHT)
		*keys ^= RIGHT;

}

t_vec3f	m4_mult_vec3f(t_mat4 mat, t_vec3f vec)
{
	t_vec3f result = (t_vec3f) {0, 0, 0};

	result.x = vec.x * mat.value[0][0] + vec.y * mat.value[0][1] + vec.z * mat.value[0][2];
	result.y = vec.x * mat.value[1][0] + vec.y * mat.value[1][1] + vec.z * mat.value[1][2];
	result.z = vec.x * mat.value[2][0] + vec.y * mat.value[2][1] + vec.z * mat.value[2][2];

	return (result);
}

void 	handle_transformation(uint32_t keys, t_vec3f *camera_position, t_vec3f *camera_rotation)
{
	t_vec3f	move 		= (t_vec3f) {0, 0, 0};

	if ((keys & FORWARD) == FORWARD)
		move.z += 0.1;
	if ((keys & BACKWARD) == BACKWARD)
		move.z -= 0.1;
	if ((keys & LEFT) == LEFT)
		move.x += 0.1;
	if ((keys & RIGHT) == RIGHT)
		move.x -= 0.1;

	if ((keys & 0x10) == 0x10)
		camera_rotation->y -= 0.1;

	move = m4_mult_vec3f(m4_rotation(0, camera_rotation->y, camera_rotation->z), move);
	*camera_position = vec_add(*camera_position, move);
}

void	handle_mouse(GLFWwindow *window, t_vec3f *camera_rotation)
{
	static double pos_x_old = 0;
	static double pos_y_old = 0;
	double pos_x;
	double pos_y;

	glfwGetCursorPos(window, &pos_x, &pos_y);

	if (pos_x_old == 0)
		pos_x_old = pos_x;
	if (pos_y_old == 0)
		pos_y_old = pos_y;

	camera_rotation->x += ((pos_y_old - pos_y) * 0.001);
	camera_rotation->y += ((pos_x_old - pos_x) * 0.001);
	pos_x_old = pos_x;
	pos_y_old = pos_y;
}

void	init_window(GLFWwindow **window, uint32_t width, uint32_t height)
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
	glfwSwapInterval(1);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// t_mat4		m4_look_at(t_vec3f from, t_vec3f to)
// {
// 	t_vec3f tmp = (t_vec3f){0, 1, 0};
// 	t_vec3f forward = vec_normalize(vec_sub(from, to));
// 	t_vec3f right = vec_cross(vec_normalize(tmp), forward);
// 	t_vec3f up = vec_cross(forward, right);
// 	t_mat4	result;
	
// 	result.value[0][0] = right.x;
//     result.value[0][1] = right.y;
//     result.value[0][2] = right.z;
//     result.value[1][0] = up.x;
//     result.value[1][1] = up.y;
//     result.value[1][2] = up.z;
//     result.value[2][0] = forward.x;
//     result.value[2][1] = forward.y;
//     result.value[2][2] = forward.z;
//     result.value[3][0] = from.x;
//     result.value[3][1] = from.y;
//     result.value[3][2] = from.z;
	
// 	return (result);
// }

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
	t_scop	*scop;

	scop = calloc(1, sizeof(t_scop));

	scop->nb_mats = 0;
	scop->nb_triangles = 0;
	scop->textures_count = 0;
	scop->textures = NULL;

	scop->keys = 0x0;
	scop->camera_position = (t_vec3f) {0, 0, 0};
	scop->camera_rotation = (t_vec3f) {0, 0, 0};
	
	scop->width = 1920;
	scop->height = 1080;

	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);

	printf("[Scop] Starting parser\n");
	parser_init(scop, argv[1]);

	printf("- Triangles count : %zu\n", scop->nb_triangles);
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