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
        |bus = 0, attackTime, releaseTime, curve|
        var out;
        out = EnvGen.kr(
            Env.perc(
                attackTime: attackTime,
                releaseTime: releaseTime,
                level: 1.0,
                curve: curve
            ),
        );
        Out.kr(bus, out)
    }
).add;

s.waitForBoot{
    var param1Left;
    var param2Left;
    var param1Right;
    var param2Right;
    
    var envelopeBusLeft = Bus.control(s, 1);
    var envelopeBusRight = Bus.control(s, 1);

    (~osc = OSCFunc(
        {
            |msg|
            if(
                msg[1] == 1,
                Synth(
                    \envelope, 
                    [
                        \bus,           envelopeBusLeft,
                        \attackTime,    msg[4],
                        \releaseTime,   msg[5],
                        \curve,         msg[6]
                    ]
                )
            );
            param1Left = msg[2];
            param2Left = msg[3];
        },"/leftColor"
    ));

    (~osc = OSCFunc(
        {
            |msg|
            if(
                msg[1] == 1,
                Synth(
                    \envelope,
                    [
                        \bus,           envelopeBusRight,
                        \attackTime,    msg[4],
                        \releaseTime,   msg[5],
                        \curve,         msg[6]
                    ]
                )
            );
            param1Right = msg[2];
            param2Right = msg[3];
        },"/rightColor"
    ));

    // crear event loop que evalua el bus que contiene los valores de
    // salida del envelope y los envia a travez de osc
    Routine(
        {
            // loop infinito
            inf.do{
                // guardar valores generados por envelope en variable
                // y mandarlos por osc
                var envelopeLeft = 0.0;
                var envelopeRight = 0.0;
                envelopeLeft = envelopeBusLeft.getSynchronous;
                ~transmiter.sendMsg("/leftColor", envelopeLeft, param1Left, param2Left);
                envelopeRight = envelopeBusRight.getSynchronous;
                ~transmiter.sendMsg("/rightColor", envelopeRight, param1Right, param2Right);
                // limitar loop al doble de la velocidad que planeo
                // transmitir video
                (1/60).wait;
            }
        }
    ).play;
}
