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
    ~transmiter.isConnected == true,
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
        |bus = 0|
        var out;
        out = EnvGen.kr(
            Env.perc(
                attackTime: 0.1,
                releaseTime: 0.5,
                level: 1.0,
                curve: -4.0
            ),
        );
        Out.kr(bus, out)
    }
).add;

s.waitForBoot{
    // crear event loop que evalua el bus que contiene los valores de
    // salida del envelope y los envia a travez de osc
    var envelopeBus = Bus.control(s, 1);


    (~osc = OSCFunc(
        {
            |msg|
            if(
                msg[1] == 1,
                Synth(\envelope,[\bus, envelopeBus])
            );
        },"/test",
    ));

    Routine(
        postln("debug");
        {
            // loop infinito
            inf.do{
                // guardar valores generados por envelope en variable
                // y mandarlos por osc
                var envelope = envelopeBus.getSynchronous;
                if(envelope > 0.0,
                    {postln(envelope)}
                );
                ~transmiter.sendMsg("/test", envelope);
                // limitar loop al doble de la velocidad que planeo
                // transmitir video
                (1/60).wait;
            }
        }
    ).play;
}
