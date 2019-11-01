#version 330
layout(location=0) in vec3 modelPosition;
layout(location=1) in vec2 uv;

out vec2 UV;

uniform mat4 VIEW;
void main() {
    UV = uv;
    gl_Position = VIEW * vec4(modelPosition, 1);
}
