#include "kek.h"

void draw_render_spatialmap(Render *render, int camera, int *entities, size_t count)
{
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        int entityid = entities[i];
        Entity *entity = get_entity(entityid);

        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {0.0f, 0.0f};
        Vec4 colormask = vec4(1,0,0,1.0);

        Vertex vertices[2];
        vertices[0].position = entity->position;
        vertices[0].colormask = colormask;
         //todo: change this magic number
        vertices[1].position.x = SPATIAL_CELL_TO_WORLD((float)entity->snode.x);
        vertices[1].position.y = SPATIAL_CELL_TO_WORLD((float)entity->snode.y);
        vertices[1].position.z = 0;
        vertices[1].colormask = colormask;

        append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));

        vertexcount += 2;
    }

    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);

    Mat4 mvp;

   get_camera_ortho_mvp(camera, &mvp);
   gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
   gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
   gl_disable(GL_DEPTH_TEST);
   gl_enable(GL_BLEND);
   gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
   if(vertexcount > 0)
       draw_vertexbuffer_lines(vb, 0, vertexcount);

}

