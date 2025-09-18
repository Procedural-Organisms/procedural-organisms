s.boot

// crear un hoste con el que mandamos los mensajes osc
// al servidos osc en el programa de OpenGL
// no me permite usar el nombre ~client
// TODO ver si me permite usar otro nombre que no sea ~host
~host = NetAddr("localhost", 11303)

// definicion de synth que al ser llamado dispara un envelope
(Ndef(
    // TODO ver si es posible usar un synthDef
    \envelope,
    {
        // normalmente se puede diparar el envelope sin cambiar 
        // el parametro "gate" de EnvGen pero en esta situacion
        // especifica se necesita mandar un cambio de parametro
        |trigger = 0|
        EnvGen.kr(
            Env.perc(
                attackTime: 0.1,
                releaseTime: 0.5,
                level: 1.0,
                curve: -4.0
            ),
            // aqui cambiamos el parametro gate de 0 a 1
            trigger
        );
    }
))

// crear event loop que evalua el bus que contiene los valores de
// salida del envelope y los envia a travez de osc
(Routine(
    {
        // loop infinito
        inf.do{
            // guardar valores generados por envelope en variable
            // y mandarlos por osc
            var envelope = Ndef(\envelope).bus.getSynchronous;
            ~host.sendMsg("/test", envelope);
            // limitar loop al doble de la velocidad que planeo
            // transmitir video
            (1/60).wait;
        }
    }
).play)

// disparar envelope
(Routine{
    // usamos el mensaje .set porque supercollider no nos permite
    // usar .play, asi que mandamos un cambio al parametro gate de
    // 0 a 1 y de vuelta a 0 para disparar envelope
    Ndef(\envelope).set(\trigger, 1);
    // esperamos el doble del tiempo que toma completar un ciclo en 
    // el event loop para cambiar el estado de vuelta a 0, asegurandonos
    // de que el event loop registre todos los triggers
    (1/30).wait;
    Ndef(\envelope).set(\trigger, 0)
}.play)


// (~osc = OSCFunc(
//     {
//         |msg|
//         if(
//             msg[1]!=0,
//             {msg[1].postln;}
//         )
//     },"/test", ~host
// ))
