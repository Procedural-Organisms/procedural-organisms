// o = Server(\funcGenServer, NetAddr("127.0.0.1", 57111));
// o.boot;

// s.boot;
// crear un hoste con el que mandamos los mensajes osc
// al servidos osc en el programa de OpenGL
// no me permite usar el nombre ~client
// TODO ver si me permite usar otro nombre que no sea ~host

// thisProcess.openUDPPort(13934);

~transmiter = NetAddr("localhost", 13939);
if(
    thisProcess.openUDPPort(13939) == true,
    postln(
        "SuperCollider OSC client ready!\nClient address: " ++
        ~transmiter.ip ++ ":" ++ ~transmiter.port
    );
);


// definicion de synth que al ser llamado dispara un envelope
SynthDef(
    // TODO ver si es posible usar un synthDef
    \envelope,
    {
        // normalmente se puede diparar el envelope sin cambiar 
        // el parametro "gate" de EnvGen pero en esta situacion
        // especifica se necesita mandar un cambio de parametro
        |bus = 0, trigger = 0|
        var out;
        out = EnvGen.kr(
            Env.perc(
                attackTime: 0.1,
                releaseTime: 0.5,
                level: 1.0,
                curve: -4.0
            ),
            // aqui cambiamos el parametro gate de 0 a 1
            trigger
        );
        Out.kr(bus, out)
    }
).add;

s.waitForBoot{
    // crear event loop que evalua el bus que contiene los valores de
    // salida del envelope y los envia a travez de osc
    var trigger = 0;
    var envelopeBus = Bus.control(s, 1);
    var envelopeSynth = Synth(\envelope,[\bus, envelopeBus, \trigger, trigger]);

    (~osc = OSCFunc(
        {
            |msg|
            if(
                msg[1]==1,
                {Routine{
                    // usamos el mensaje .set porque supercollider no nos permite
                    // usar .play, asi que mandamos un cambio al parametro gate de
                    // 0 a 1 y de vuelta a 0 para disparar envelope
                    trigger = 1;
                    // esperamos el doble del tiempo que toma completar un ciclo en 
                    // el event loop para cambiar el estado de vuelta a 0, asegurandonos
                    // de que el event loop registre todos los triggers
                    (1/30).wait;
                    trigger = 0;
                }.play}
            );
            postln("debug")
        },"/test",
    ));

    Routine(
        {
            // loop infinito
            inf.do{
                // guardar valores generados por envelope en variable
                // y mandarlos por osc
                var envelope = envelopeBus.getSynchronous;
                ~transmiter.sendMsg("/test", envelope);
                // limitar loop al doble de la velocidad que planeo
                // transmitir video
                (1/60).wait;
            }
        }
    ).play;
}
