/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glfw_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 00:02:39 by slopez            #+#    #+#             */
/*   Updated: 2021/06/18 15:45:28 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>

void	handle_keyboard(GLFWwindow *window, uint32_t keys[349])
{
    const float     glfw_time       = glfwGetTime();
    static float    key_timeout     = 0;
    const float     time_diff       = key_timeout == 0 || glfw_time - key_timeout > 0.3f;

    static uint32_t render_option   = GL_FILL;
    static uint8_t  gl_cull_face    = 1;
    static uint8_t  gl_depth_test   = 1;
    
    keys[GLFW_KEY_W] = glfwGetKey(window, GLFW_KEY_W);
    keys[GLFW_KEY_A] = glfwGetKey(window, GLFW_KEY_A);
    keys[GLFW_KEY_S] = glfwGetKey(window, GLFW_KEY_S);
    keys[GLFW_KEY_D] = glfwGetKey(window, GLFW_KEY_D);
    keys[GLFW_KEY_R] = glfwGetKey(window, GLFW_KEY_R);
    keys[GLFW_KEY_F] = glfwGetKey(window, GLFW_KEY_F);
    keys[GLFW_KEY_LEFT_SHIFT] = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        exit (1);

    if (glfwGetKey(window, GLFW_KEY_2))
        render_option = GL_LINE;
    if (glfwGetKey(window, GLFW_KEY_3))
        render_option = GL_POINT;
    if (glfwGetKey(window, GLFW_KEY_1))
        render_option = GL_FILL;

    if (glfwGetKey(window, GLFW_KEY_4) && time_diff)
    {
        if (!gl_cull_face)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
        gl_cull_face = !gl_cull_face;
        key_timeout = glfw_time;
    }
    if (glfwGetKey(window, GLFW_KEY_5) && time_diff)
    {
        if (!gl_depth_test)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        gl_depth_test = !gl_depth_test;
        key_timeout = glfw_time;
    }
    glPolygonMode(GL_FRONT_AND_BACK, render_option);
}

void	handle_mouse(GLFWwindow *window, t_vec3f *cam_rot)
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

	cam_rot->x += ((pos_y_old - pos_y) * 0.001);
	cam_rot->y += ((pos_x_old - pos_x) * 0.001);
	pos_x_old = pos_x;
	pos_y_old = pos_y;
}

void 	handle_transformations(uint32_t keys[349], t_vec3f *cam_pos, t_vec3f *cam_rot, float multiplier)
{
	t_vec3f	move 		= (t_vec3f) {0, 0, 0};

    if (keys[GLFW_KEY_W])
	    move.z += 0.1;
    if (keys[GLFW_KEY_S])
	    move.z -= 0.1;
    if (keys[GLFW_KEY_R])
	    move.y += 0.1;
    if (keys[GLFW_KEY_F])
	    move.y -= 0.1;
    if (keys[GLFW_KEY_A])
	    move.x += 0.1;
    if (keys[GLFW_KEY_D])
	    move.x -= 0.1;
    if (keys[GLFW_KEY_LEFT_SHIFT])
        move = vec_multf(move, multiplier);
        
   
	move = m4_mult_vec3f(m4_rotation(0, cam_rot->y, 0), move);
	*cam_pos = vec_add(*cam_pos, move);
}