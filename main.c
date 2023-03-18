#define SOKOL_IMPL
#define SOKOL_GLCORE33

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "thirdparty/sokol_app.h"
#include "thirdparty/sokol_gfx.h"
#include "thirdparty/sokol_log.h"
#include "thirdparty/sokol_glue.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

sg_pass_action pass_action;

void init(void) {
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });
    pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .value={1.0f, 0.0f, 0.0f, 1.0f}}
    };
}


static void event(const sapp_event* e) {
    
    #if 1
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
    #endif

}

void frame(void) {

    
    #if 0
    float g = pass_action.colors[0].value.g + 0.01f;
    pass_action.colors[0].value.g = (g > 1.0f) ? 0.0f : g;
     
    sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
    sg_end_pass();
    #endif 
}

void cleanup(void) {

    printf("[DEBUG] Freeing memory.\n");
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
        .win32_console_attach = true,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
