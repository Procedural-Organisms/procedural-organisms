#version 330 core
out vec4 FragColor;


//= INPUTS ==
// from shaders
in vec3 gradient;


//= MAIN ==
void main(){
    FragColor = vec4(gradient, 1.0);
}
