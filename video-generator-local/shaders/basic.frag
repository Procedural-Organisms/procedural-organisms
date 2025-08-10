#version 330 core
out vec4 FragColor;

in vec4 gradientGenerator1;
in vec4 gradientGenerator2;
in vec4 flashGenerator1;
in vec4 flashGenerator2;
in float xCoordinates;
uniform float sinGenerator;

void main(){
    if(xCoordinates < 0){
        FragColor = (1 - gradientGenerator1) * flashGenerator1 + gradientGenerator1;
    }
    else{
        FragColor = (1 - gradientGenerator2) * flashGenerator2 + gradientGenerator2;
    }
}
