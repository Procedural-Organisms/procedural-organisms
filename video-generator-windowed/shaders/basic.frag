#version 330 core
out vec4 FragColor;

//= INPUTS ==
// from shaders
in vec3 gradient;


void main(){
    // FragColor = vec4(envelopeLeft * 0.95, envelopeRight * 0.75, 0.65, 1.0);

    // FragColor = vec4(hsl2rgb(vec3(
    //     param2Right,
    //     envelopeRight * param1Right * 0.3,
    //     (1 - (envelopeRight * param2Right)) * (0.4) + 0.3
    // )), 1.0);

    FragColor = vec4(gradient, 1.0);
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
