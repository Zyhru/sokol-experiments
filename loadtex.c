#define SOKOL_IMPL
#define SOKOL_GLCORE33

#include "thirdparty/sokol_app.h"
#include "thirdparty/sokol_gfx.h"
#include "thirdparty/sokol_log.h"
#include "thirdparty/sokol_glue.h"
#include "zaitypes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "thirdparty/HandmadeMath.h"

#include "quad-glsl.h"

static struct {
    sg_pipeline pipe;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

f32 vertex_data[] = {
    
    // quad position      uv 
    0.5f, 0.5f, 0.0f,    1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  
    -0.5f, 0.5f, 0.0f,   0.0f, 1.0f
};

u16 idx_buf[] = {
    0,1,3,
    1,2,3
};

void init(void) { 
    sg_setup(&(sg_desc) {
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });

    // load image from CPU
    i32 w, h, num_channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *pixels = stbi_load("naruto.png",
                                &w,
                                &h,
                                &num_channels,
                                4);
    // for now we're going to render our quad
    sg_buffer v_buf = sg_make_buffer(&(sg_buffer_desc) {
                                     .data = SG_RANGE(vertex_data)
    });
    
    sg_buffer i_buf = sg_make_buffer(&(sg_buffer_desc) {
                                     .type = SG_BUFFERTYPE_INDEXBUFFER,
                                     .data = SG_RANGE(idx_buf)
    });

   // shader time 
    sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));

    // gpu pipeline 
    state.pipe = sg_make_pipeline(&(sg_pipeline_desc) {
                                .layout = {
                                    .attrs = {
                                        [0].format = SG_VERTEXFORMAT_FLOAT3,
                                        [1].format = SG_VERTEXFORMAT_FLOAT2
                                    }
                                },
                                .shader = shd,
                                .index_type = SG_INDEXTYPE_UINT16,
                                .depth = {
                                    .write_enabled = true,
                                    .compare = SG_COMPAREFUNC_LESS_EQUAL,
                                },
                                .label = "quad-pipeline"
                                
    });
    
    state.bind = (sg_bindings) {
        .vertex_buffers[0] = v_buf,
        .index_buffer = i_buf
    };
    
    #if 1
    sg_image image = sg_make_image(&(sg_image_desc) {
                                   .width = w,
                                   .height = h,
                                   .pixel_format = SG_PIXELFORMAT_RGBA8,
                                   .min_filter = SG_FILTER_LINEAR,
                                   .mag_filter = SG_FILTER_LINEAR,
                                   .data.subimage[0][0] = {
                                    .ptr = pixels,
                                    .size = (size_t)(w * h * 4),
                                   }

    });
    state.bind.fs_images[SLOT_tex] = image;
    stbi_image_free(pixels);
    #endif
}

void frame(void) { 
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .value={0.0f, 0.0f, 0.0f, 1.0f}}
    };
    
    sg_begin_default_pass(&state.pass_action, 800, 600);
    sg_apply_pipeline(state.pipe);
    sg_apply_bindings(&state.bind);
    sg_draw(0,6,1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        //.event_cb = event,
        .width = 800.0f,
        .height = 600.0f, 
        .window_title = "untitled",
        .gl_force_gles2 = true,
        .win32_console_attach = true,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}


