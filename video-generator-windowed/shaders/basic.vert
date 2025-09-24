#version 330 core

//= INPUTS ==
// buffer with position vectors
layout(location = 0) in vec3 aPos;


//= OUTPUTS ==
// __(code)__


//= MAIN ==
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}


// out vec4 flashGenerator1;
// out vec4 flashGenerator2;

// // ===  TEST  ===
// uniform float testLeft;
// uniform float testRight;
// // == == == == ==

// void main(){
//     gl_Position = vec4(
//         aPos.x,
//         aPos.y,
//         aPos.z,
//         1.0f
//         );

//     // ===  TEST  ===
//     flashGenerator1 = vec4(
//         testLeft * 0.3f,
//         testLeft * 0.3f,
//         testLeft * 0.3f,
//         1.0f
//     );

//     flashGenerator2 = vec4(
//         testRight * 0.3f,
//         testRight * 0.3f,
//         testRight * 0.3f,
//         1.0f
//     );
//     // == == == == ==

//     xCoordinates = aPos.x;
// }
