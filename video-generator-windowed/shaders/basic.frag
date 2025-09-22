#version 330 core
out vec4 FragColor;

void main(){
    FragColor = vec4(0.95, 0.75, 0.65, 1.0);
}

// in vec4 flashGenerator1;
// in vec4 flashGenerator2;
// in float xCoordinates;

// void main(){
//     if(xCoordinates < 0){
//         FragColor = (1 - gradientGenerator1) * flashGenerator1 + gradientGenerator1;
//     }
//     else{
//         FragColor = (1 - gradientGenerator2) * flashGenerator2 + gradientGenerator2;
//     }
// }
