#include "kek.h"

void draw_render_lines_default(Render *render, int camera, Vec3 *points, size_t count, void *ctx)
{
    (void)ctx;
    size_t drawcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);
    for(size_t i = 0; i < count; i += 2)
    {
        Vertex vertices[2];
        vertices[0].position = points[i];
        vertices[1].position = points[i+1];
        vertices[0].uv = vec2(0,0);
        vertices[1].uv = vec2(0,0);
        vertices[0].colormask = vec4(1,1,1,1);
        vertices[1].colormask = vec4(1,1,1,1);
        vertices[0].normal = vec3(0,0,1);
        vertices[1].normal = vec3(0,0,1);

        append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));
    }

    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);
    for(size_t i = 0; i < drawcount; ++i)
    {
        Mat4 mvp;

        get_camera_ortho_mvp(camera, &mvp);
        gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
        gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
        gl_disable(GL_DEPTH_TEST);
        gl_enable(GL_BLEND);
        gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    draw_vertexbuffer_line_strip(vb, 0, count);

}

