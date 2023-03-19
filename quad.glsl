@vs vs

in vec3 position;
//in vec3 color0;
in vec2 texcoord0;

out vec2 uv;
//out vec3 color;

void main() {
    gl_Position = vec4(position, 1.0f);
 //   color = color0;
    uv = texcoord0;
}
@end

@fs fs
//in vec3 color;
in vec2 uv;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = texture(tex, uv); 
}
@end

@program quad vs fs
