#version 330 core
out vec4 FragColor;

in vec4 gradientGenerator;
uniform float sinGenerator;

void main(){
    FragColor = gradientGenerator;
}
