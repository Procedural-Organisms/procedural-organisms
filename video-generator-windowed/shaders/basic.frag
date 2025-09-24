#version 330 core
out vec4 FragColor;

//= INPUTS ==
// uniforms
uniform float envelopeLeft;
uniform float param1Left;
uniform float param2Left;
//=:=:=:=:=:=:=:=:=:=:=
uniform float envelopeRight;
uniform float param1Right;
uniform float param2Right;



//= FUNCTIONS ==
// hsl to rgb conversion
vec3 hsl2rgb(in vec3 c)
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}


void main(){
    // FragColor = vec4(envelopeLeft * 0.95, envelopeRight * 0.75, 0.65, 1.0);
    FragColor = vec4(hsl2rgb(vec3(
        param2Left,
        envelopeLeft * param1Left * 0.3,
        (1 - (envelopeLeft * param2Left)) * (0.4) + 0.3
    )), 1.0);
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
