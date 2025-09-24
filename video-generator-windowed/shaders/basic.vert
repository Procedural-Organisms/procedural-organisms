#version 330 core


//= INPUTS ==
// buffer with position vectors
layout(location = 0) in vec3 aPos;
// uniforms
uniform float envelopeLeft;
uniform float param1Left;
uniform float param2Left;
//=:=:=:=:=:=:=:=:=:=:=
uniform float envelopeRight;
uniform float param1Right;
uniform float param2Right;


//= OUTPUTS ==
out vec3 gradient;


//= FUNCTIONS ==
// hsl to rgb conversion
vec3 hsl2rgb(in vec3 c)
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}


//= MAIN ==
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    if(aPos.x < 0){
        gradient = vec3(hsl2rgb(vec3(
            param2Left * (-aPos.x),
            envelopeLeft * param1Left * 0.3 * aPos.y,
            (1 - (envelopeLeft * param2Left)) * (0.4) + 0.3 * (-aPos.x)
        )));
    }
    else{
        gradient = vec3(hsl2rgb(vec3(
            param2Right * aPos.x,
            envelopeRight * param1Right * 0.3 * (aPos.y),
            (1 - (envelopeRight * param2Right)) * (0.4) + 0.3 * aPos.x
        )));
    }
}
