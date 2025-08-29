Server.default.options.outDevice_("MacBook Pro Speakers")
Server.default.options.outDevice_("External Headphones")
Server.default.options.outDevice_("BlackHole 2ch")

s.boot

thisProcess.openUDPPort(11301);

(
SynthDef("perc", {
    arg carfreq = 89.0, modfreq = 365.0, pan = 0;
    var out, envelope = Env.perc(0.01, 0.2, 0.8, -10.0);
    out = PMOsc.ar(
        carfreq: carfreq,
        modfreq: modfreq,
        pmindex: EnvGen.kr(envelope),
        mul: EnvGen.kr(envelope, doneAction: 2)
    );
    Out.ar(pan, out)
}).add
)

(
~lowPerc = OSCFunc({
    arg msg;
    if(msg[1] == 1,
        Synth("perc", [
            "carfreq", 194.0,
            "modfreq", 97.0,
            "pan", 1
        ])
    )
}, '/hiPerc/trigger');
)

(
~hiPerc = OSCFunc({
    arg msg;
    if(msg[1] == 1,
        Synth("perc", [
            "carfreq", 89.0,
            "modfreq", 365.0,
            "pan", 0
        ])
    )
}, '/lowPerc/trigger')
)


~lowPerc.free;
~hiPerc.free;



(
Synth("perc", [
            "carfreq", 194.0,
            "modfreq", 97.0,
            "pan", 1
        ])
)








play({
    PMOsc.ar(
        carfreq: [158.0, 332.0],
        modfreq: [83.0, 79.0],
        pmindex: [
            SinOsc.ar(freq: 0.083, mul: 0.2, add: 0.1),
            SinOsc.ar(freq: 0.079, mul: 0.2, add: 0.1)
        ],
        mul: [
            SinOsc.ar(freq: 0.079, mul: 0.3, add: 0.05),
            SinOsc.ar(freq: 0.083, mul: 0.3, add: 0.05)
        ]
    )
})