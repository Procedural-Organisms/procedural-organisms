s.options.hardwareBufferSize = 4096;
s.options.blockSize = 512;
s.options.sampleRate = 48000;
s.options.recHeaderFormat = "raw";
s.options.recSampleFormat = "int16";
// TODO hacer canales de salida 0
s.options.numOutputBusChannels = 2;
s.options.numInputBusChannels = 0;
s.options.maxLogins = 3;
s.boot;

SynthDef("perc", {
    | carfreq = 89.0, modfreq = 365.0, pan = 0,
    attack = 0.01, release = 0.2, curve = -10.0 |

    var out, envelope = Env.perc(attack, release, 0.8, curve);
    out = PMOsc.ar(
        carfreq: carfreq,
        modfreq: modfreq,
        pmindex: EnvGen.kr(envelope),
        mul: EnvGen.kr(envelope, doneAction: 2)
    );
    Out.ar(pan + 100, out)
}).add;

~leftPerc = OSCFunc({
    arg msg;

    var carfreqOSC = msg[2];
    var modfreqOSC = msg[3];
    var attackOSC  = msg[4];
    var releaseOSC = msg[5];
    var curveOSC   = msg[6];

    if(msg[1] == 1,
        Synth("perc", [
            "carfreq", carfreqOSC,
            "modfreq", modfreqOSC,
            "attack", attackOSC,
            "release", releaseOSC,
            "curve", curveOSC,
            "pan", 0
        ])
    )
}, '/leftPerc');

~rightPerc = OSCFunc({
    arg msg;

    var carfreqOSC = msg[2];
    var modfreqOSC = msg[3];
    var attackOSC  = msg[4];
    var releaseOSC = msg[5];
    var curveOSC   = msg[6];

    if(msg[1] == 1,
        Synth("perc", [
            "carfreq", carfreqOSC,
            "modfreq", modfreqOSC,
            "attack", attackOSC,
            "release", releaseOSC,
            "curve", curveOSC,
            "pan", 1
        ])
    )
}, '/rightPerc');


s.waitForBoot{
    Routine{
        // TODO reactivar deteccion de flag
        // var flagPath = ~video_started;
        // while {File.exists(flagPath).not}{0.1.wait};
        // "Video started, starting audio and transmition.".postln;
        
        Synth("perc", [
            "carfreq", 194.0,
            "modfreq", 97.0,
            "pan", 1
        ]);
        Synth("perc", [
            "carfreq", 89.0,
            "modfreq", 365.0,
            "pan", 0
        ]);

        // correccion de latencia
        {DelayN.ar(In.ar(100, 2), 5.0, 0.2)}.play();

        // transmitir a fifo
        // TODO reactivar grabacion a fifo
        // s.record(path: ~audio_pipe, numChannels: 2);
    }.play;
};
