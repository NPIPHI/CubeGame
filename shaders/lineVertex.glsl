#version 330
layout(location=0) in vec3 modelPosition;

out vec3 fragColor;

uniform mat4 VIEW;
uniform vec3 OFFSET;

void main() {
    gl_Position = VIEW * vec4(modelPosition + OFFSET, 1);
    fragColor = vec3(1, 0, 0);
}
