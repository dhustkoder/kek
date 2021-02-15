#include "kek.h"

void draw_render_spatialmap(Render *render, Camera *camera, Entity **entities, size_t count)
{
    size_t vertexcount = 0;
    VertexBuffer *vb = render->vb;
    GLuint shader = render->shader->shader;

    map_vertex_buffer(vb);
    clear_vertex_buffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        Entity *entity = entities[i];

        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {0.0f, 0.0f};
        Vec4 colormask = vec4(1,1,1,1);

        Vertex vertices[2];
        vertices[0].position = entity->position;
#warning MAGIC NUMBER
        vertices[1].position = vec3(1024*entity->snode.x, 1024*entity->snode.y, 0);

        append_vertex_buffer(vb, (uint8_t *)vertices, sizeof(vertices));

        vertexcount += 2;
    }

    unmap_vertex_buffer(vb);

    bind_vertex_buffer(vb);
    bind_shader(render->shader);

    Mat4 mvp;

   get_camera_ortho_mvp(camera, &mvp);
   gl_uint umvp = gl_get_uniform_location(shader, "u_mvp");
   gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
   gl_disable(GL_DEPTH_TEST);
   gl_enable(GL_BLEND);
   gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
   if(vertexcount > 0)
       draw_vertex_buffer_lines(vb, 0, vertexcount);

}

