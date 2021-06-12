/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glfw_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 00:02:39 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 00:03:07 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

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
