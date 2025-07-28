#version 330 core
layout(location = 0) in vec3 aPos;

out vec4 gradientGenerator;
uniform float sinGenerator;

void main(){
    gl_Position = vec4(
        aPos.x,
        aPos.y,
        1.0f,
        1.0f
        );
    gradientGenerator = vec4(
        (-aPos.x / 2.0f + 0.5f) * (sinGenerator / 2.0f + 0.5),
        (aPos.y / 2.0f + 0.5) * sinGenerator,
        (aPos.x / 2.0f + 0.5f) * (sinGenerator / 5.0f +0.8f),
        1.0f
        );
}
