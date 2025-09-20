#version 330 core
layout(location = 0) in vec3 aPos;

out vec4 gradientGenerator1;
out vec4 gradientGenerator2;
out vec4 flashGenerator1;
out vec4 flashGenerator2;
out float xCoordinates;
uniform float sinGenerator1;
uniform float sinGenerator2;
// TODO cambiar nombre de variables rampGenerator
uniform float rampGenerator1;
uniform float rampGenerator2;

// ===  TEST  ===
uniform float test;
// == == == == ==

void main(){
    gl_Position = vec4(
        aPos.x,
        aPos.y,
        aPos.z,
        1.0f
        );

    gradientGenerator1 = vec4(
        (-aPos.x * 0.5f + 0.5f) * (sinGenerator1 * 0.5f + 0.5f),
        (aPos.y * 0.4f + 0.1f) * sinGenerator1,
        (aPos.x * 0.5f + 0.5f) * (sinGenerator1 * 0.2f + 0.8f),
        1.0f
        );

    gradientGenerator2 = vec4(
        aPos.x * 0.2f + 0.5f * (sinGenerator2 * 0.5f + 0.5f),
        aPos.x * aPos.y * 0.4f + 0.3f * (sinGenerator2 * 0.5f + 0.5f),
        aPos.y * 0.2f + 0.5f * (sinGenerator2 * 0.5f + 0.5f),
        1.0f
    );

    flashGenerator1 = vec4(
        rampGenerator1 * 0.3f,
        rampGenerator1 * 0.3f,
        rampGenerator1 * 0.3f,
        1.0f
    );

    // ===  TEST  ===
    flashGenerator1 = vec4(
        test * 0.3f,
        test * 0.3f,
        test * 0.3f,
        1.0f
    );
    // == == == == ==

    flashGenerator2 = vec4(
        rampGenerator2 * 0.3f,
        rampGenerator2 * 0.3f,
        rampGenerator2 * 0.3f,
        1.0f
    );

    xCoordinates = aPos.x;
}
