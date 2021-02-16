#include "kek.h"

void draw_render_lines_default(Render *render, int camera, Vec3 *points, size_t count, void *ctx)
{
    size_t drawcount = 0;
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertex_buffer(vb);
    clear_vertex_buffer(vb);
    for(int i = 0; i < count; i += 2)
    {
        Vec4 colormask = {1,1,1,1};
        Vertex vertices[2];
        vertices[0].position = points[i];
        vertices[1].position = points[i+1];
        vertices[0].uv = vec2(0,0);
        vertices[1].uv = vec2(0,0);
        vertices[0].colormask = vec4(1,1,1,1);
        vertices[1].colormask = vec4(1,1,1,1);
        vertices[0].normal = vec3(0,0,1);
        vertices[1].normal = vec3(0,0,1);

        append_vertex_buffer(vb, (uint8_t *)vertices, sizeof(vertices));
    }

    unmap_vertex_buffer(vb);

    bind_vertex_buffer(vb);
    bind_shader(render->shader);
    for(int i = 0; i < drawcount; ++i)
    {
        Mat4 mvp;

        get_camera_ortho_mvp(camera, &mvp);
        gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
        gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
        gl_disable(GL_DEPTH_TEST);
        gl_enable(GL_BLEND);
        gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    draw_vertex_buffer_line_strip(vb, 0, count);

}

