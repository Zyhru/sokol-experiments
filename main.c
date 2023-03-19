#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define _CRT_SECURE_NO_WARNINGS
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

#include "cube.glsl.h"

#define _DEBUG 0

static struct {
    f32 rx, ry;
    sg_pipeline pipe;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

float vertices[] = {
     // coords            // color
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f
};

// initializing shit
void init(void) {
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });
 
    // load image data from CPU
    i32 img_w, img_h, num_channels;
    stbi_uc *image = stbi_load("computer.png", 
                               &img_w, 
                               &img_h,
                               &num_channels,
                               4);

    if(!image) {
        printf("Image could not be loaded\n");
        return;
    }
    
    // Creates Texture

    #if 0
    sg_image computer_image = sg_make_image( &(sg_image_desc) {
          .width = img_w,
          .height = img_h,
          .pixel_format = SG_PIXELFORMAT_R8,
          .min_filter = SG_FILTER_LINEAR,
          .mag_filter = SG_FILTER_LINEAR,
          .data.subimage[0][0] = {
                .ptr = image,
                .size = (size_t)(img_w * img_h * 4),
            }
    });
    stbi_image_free(image);
    #endif
     
    sg_buffer vertex_buffer = sg_make_buffer(&(sg_buffer_desc) { 
                .data = SG_RANGE(vertices)
    });

    sg_shader shd = sg_make_shader(cube_shader_desc(sg_query_backend()));

    // 4. init pipeline
    state.pipe = sg_make_pipeline(&(sg_pipeline_desc) {
                                        .layout = {
                                            .attrs = {
                                                [0].format = SG_VERTEXFORMAT_FLOAT3,
                                                [1].format = SG_VERTEXFORMAT_FLOAT3
                                            }
                                        },
                                        .shader = shd,             
                                        .depth = {
                                            .write_enabled = true,
                                            .compare = SG_COMPAREFUNC_LESS_EQUAL,
                                        },
                                        .label = "cube-pipeline"
                                        
    });

    // 5. bind resources
    state.bind.vertex_buffers[0] = vertex_buffer;
    
    
}

// listens for input
void event(const sapp_event* e) { 
    switch(e->type) {
        case SAPP_EVENTTYPE_KEY_DOWN:
           if(e->key_code == SAPP_KEYCODE_ESCAPE) {
            printf("Closing Application\n");
            sapp_quit();
        }
            break;
        default:
            break;
    } 
}

// rendering  
void frame(void) {
    vs_params_t vs_params;
    const f32 time = (f32)(sapp_frame_duration()); 
    
    #if _DEBUG
    /* Transformation Matrices */

    Mat4 projection_matrix = Perspective_RH_ZO(ToRad(45.0f), 800.0f / 600.0f , 0.01f, 100.0f);

    // The position of the camera
    Mat4 view_matrix = LookAt_RH(V3(5.0f, 1.0f, -0.5f), V3(0.0f, 0.0f, 0.0f), V3(0.0f, 1.0f, 0.0f));
    Mat4 view_proj = MulM4(projection_matrix, view_matrix);
    
    // Model Matrix <- Think of it as changing the rotation/position or scaling your player
    state.rx += 1.0f * time, state.ry += 2.0f * time;
    Mat4 rotate_x = Rotate_RH(state.rx, V3(1.0f, 0.0f, 0.0f));
    Mat4 rotate_y = Rotate_RH(state.ry, V3(0.0f, 1.0f, 0.0f));
    Mat4 model = MulM4(rotate_x, rotate_y);
    vs_params.mvp = MulM4(view_proj, model); 
    #endif

    // clearing the screen
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .value={1.0f, 0.0f, 0.0f, 1.0f}}
    };

    sg_begin_default_pass(&state.pass_action, 800.0f, 600.0f); 
    sg_apply_pipeline(state.pipe); 
    sg_apply_bindings(&state.bind);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
    sg_draw(0, 36, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    printf("Shutting down...\n");
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,
        .window_title = "untitled",
        .gl_force_gles2 = true,
        .win32_console_attach = true,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
