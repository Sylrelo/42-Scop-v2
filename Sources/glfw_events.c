/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glfw_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 00:02:39 by slopez            #+#    #+#             */
/*   Updated: 2021/06/23 12:19:21 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <math.h>

static void         protect_rotation_overflow(t_vec3f *rot)
{
    const float     twopi = 2 * M_PI;

    if (rot->x <= -twopi || rot->x >= twopi)
        rot->x = 0;
    if (rot->y <= -twopi || rot->y >= twopi)
        rot->y = 0;
    if (rot->z <= -twopi || rot->z >= twopi)
        rot->z = 0;
}

void                handle_keyboard(GLFWwindow *window, uint32_t keys[349], int *s_texturing, int *s_mapping, short *selected_object, size_t objects_count)
{
    const float     glfw_time       = glfwGetTime();
    static float    key_timeout     = 0;
    const float     time_diff       = key_timeout == 0 || glfw_time - key_timeout > 0.3f;
    static uint32_t render_option   = GL_FILL;
    static uint8_t  gl_cull_face    = 1;
    static uint8_t  gl_depth_test   = 1;
    
    keys[GLFW_KEY_W]            = glfwGetKey(window, GLFW_KEY_W);
    keys[GLFW_KEY_A]            = glfwGetKey(window, GLFW_KEY_A);
    keys[GLFW_KEY_S]            = glfwGetKey(window, GLFW_KEY_S);
    keys[GLFW_KEY_D]            = glfwGetKey(window, GLFW_KEY_D);
    keys[GLFW_KEY_R]            = glfwGetKey(window, GLFW_KEY_R);
    keys[GLFW_KEY_F]            = glfwGetKey(window, GLFW_KEY_F);
    keys[GLFW_KEY_T]            = glfwGetKey(window, GLFW_KEY_T);
    keys[GLFW_KEY_LEFT_SHIFT]   = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    keys[GLFW_KEY_LEFT]         = glfwGetKey(window, GLFW_KEY_LEFT);
    keys[GLFW_KEY_RIGHT]        = glfwGetKey(window, GLFW_KEY_RIGHT);
    keys[GLFW_KEY_UP]           = glfwGetKey(window, GLFW_KEY_UP);
    keys[GLFW_KEY_DOWN]         = glfwGetKey(window, GLFW_KEY_DOWN);
    keys[GLFW_KEY_PERIOD]       = glfwGetKey(window, GLFW_KEY_PERIOD);
    keys[GLFW_KEY_COMMA]        = glfwGetKey(window, GLFW_KEY_COMMA);
    keys[GLFW_KEY_MINUS]        = glfwGetKey(window, GLFW_KEY_MINUS);
    keys[GLFW_KEY_EQUAL]        = glfwGetKey(window, GLFW_KEY_EQUAL);

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
    if (glfwGetKey(window, GLFW_KEY_T) && time_diff)
    {
        (*s_texturing)++;
        if (*s_texturing >= 5)
            *s_texturing = 0;
        key_timeout = glfw_time + 0.8;
        dprintf(1, "Texture mode changed : ");
        switch (*s_texturing)
        {
            case 0:
                printf("Material colors\n");
                break ;
            case 1:
                printf("Material texture(s)\n");
                break ;
            case 2:
                printf("Auto UV texturing\n");
                break ;
            case 3:
                printf("Black and white triangles\n");
                break ;
            case 4:
                printf("Colored triangles\n");
                break ;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_M) && time_diff)
    {
        (*s_mapping)++;
        if (*s_mapping >= 4)
            *s_mapping = 0;
        key_timeout = glfw_time;
        dprintf(1, "UV mapping changed : ");
        switch (*s_mapping)
        {
            case 0:
                printf("Object's UV\n");
                break ;
            case 1:
                printf("Cylinder UV\n");
                break ;
            case 2:
                printf("Sphere UV\n");
                break ;
            case 3:
                printf("Position\n");
                break ;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_O) && time_diff)
    {
        (*selected_object)++;
        if ((size_t) *selected_object >= objects_count)
            *selected_object = -1;
        key_timeout = glfw_time - .15;
    }
    glPolygonMode(GL_FRONT_AND_BACK, render_option);
}

void                handle_mouse(GLFWwindow *window, t_vec3f *cam_rot)
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
    protect_rotation_overflow(cam_rot);
	pos_x_old = pos_x;
	pos_y_old = pos_y;
}

static void         rotate_object(t_scop *scop, t_vec3f rot, float scale, float multiplier)
{
    if (scop->selected_object == -1)
    {
        for (size_t i = 0; i < scop->objects_count; i++)
        {
            scop->objects[i].rot.x += rot.x * multiplier;
            scop->objects[i].rot.y += rot.y * multiplier;
            scop->objects[i].rot.z += rot.z * multiplier;
            scop->objects[i].scale += scale * multiplier;
            if (scop->objects[i].scale <= 0)
                scop->objects[i].scale = 0;
            protect_rotation_overflow(&scop->objects[i].rot);
        }
    }
    else
    {
        scop->objects[(size_t) scop->selected_object].scale += scale * multiplier;
        scop->objects[(size_t) scop->selected_object].rot.x += rot.x * multiplier;
        scop->objects[(size_t) scop->selected_object].rot.y += rot.y * multiplier;
        scop->objects[(size_t) scop->selected_object].rot.z += rot.z * multiplier;
        protect_rotation_overflow(&scop->objects[(size_t) scop->selected_object].rot);
        if (scop->objects[(size_t) scop->selected_object].scale <= 0)
            scop->objects[(size_t) scop->selected_object].scale = 0;
    }
}

void                handle_transformations(t_scop *scop)
{
    const uint32_t *keys    = scop->keys;
	t_vec3f	move 		    = (t_vec3f) {0, 0, 0};
    float multiplier        = 1;

    if (keys[GLFW_KEY_W])
	    move.z += 0.1;
    if (keys[GLFW_KEY_S])
	    move.z -= 0.1;
    if (keys[GLFW_KEY_R])
	    move.y -= 0.1;
    if (keys[GLFW_KEY_F])
	    move.y += 0.1;
    if (keys[GLFW_KEY_A])
	    move.x += 0.1;
    if (keys[GLFW_KEY_D])
	    move.x -= 0.1;
    if (keys[GLFW_KEY_LEFT_SHIFT])
    {
        multiplier = scop->multiplier;
        move = vec_multf(move, scop->multiplier);
    }

    if (keys[GLFW_KEY_LEFT])
        rotate_object(scop, (t_vec3f){0, 0.01, 0}, 0, multiplier);
    if (keys[GLFW_KEY_RIGHT])
        rotate_object(scop, (t_vec3f){0, -0.01, 0}, 0, multiplier);
    if (keys[GLFW_KEY_UP])
        rotate_object(scop, (t_vec3f){0.01, 0, 0}, 0, multiplier);
    if (keys[GLFW_KEY_DOWN])
        rotate_object(scop, (t_vec3f){-0.01, 0, 0}, 0, multiplier);
    if (keys[GLFW_KEY_COMMA])
        rotate_object(scop, (t_vec3f){0, 0, 0.01}, 0, multiplier);
    if (keys[GLFW_KEY_PERIOD])
        rotate_object(scop, (t_vec3f){0, 0, -0.01}, 0, multiplier);
    if (keys[GLFW_KEY_MINUS])
        rotate_object(scop, (t_vec3f){0, 0, 0}, -0.01, multiplier);
    if (keys[GLFW_KEY_EQUAL])
        rotate_object(scop, (t_vec3f){0, 0, 0}, 0.01, multiplier);

    if (scop->selected_object != -1)
    {
        scop->objects[(size_t) scop->selected_object].pos.x -= move.x;
        scop->objects[(size_t) scop->selected_object].pos.y -= move.y;
        scop->objects[(size_t) scop->selected_object].pos.z -= move.z;
    }
    else
    {
        move = m4_mult_vec3f(m4_rotation(scop->cam_rot.x, scop->cam_rot.y, 0), move);
        scop->cam_pos = vec_add(scop->cam_pos, move);
    }
}