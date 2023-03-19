#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 frag_color;

void main() {

  // convert object to 3D 
  gl_Position = projection * view * model * position;
  frag_color = color;
}

