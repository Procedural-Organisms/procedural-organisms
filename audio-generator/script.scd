s.options.hardwareBufferSize = 4096;
s.options.blockSize = 512;
s.options.sampleRate = 44100;
s.options.recHeaderFormat = "raw";
s.options.recSampleFormat = "int16";
s.options.numOutputBusChannels = 2;
s.options.numInputBusChannels = 0;
s.boot;

~audio_ready = "temp/flags/audio_ready.flag";
~video_started = "temp/flags/video_started.flag";
~audio_pipe = "temp/pipes/audio_pipe";

s.waitForBoot{
    // crear flag de inicializacion de servidor
    File(~audio_ready, "w").close;
    "Audio server ready. Waiting for video.".postln;

    // esperar a flag de inicializacion de video
    Routine{
        var flagPath = ~video_started;
        while {File.exists(flagPath).not}{0.1.wait};
        "Video started, starting audio and transmition.".postln;

        // inicializar de sintetizador
        (play({
            PMOsc.ar(
                carfreq: [89.0, 194.0],
                modfreq: [356.0, 97.0],
                pmindex: [
                    Saw.ar(freq: 0.291, mul: 1, add: 1),
                    Saw.ar(freq: 0.267, mul: 1, add: 2)
                ],
                mul: [
                    Saw.ar(freq: 0.291, mul: 0.002, add: 0.002),
                    Saw.ar(freq: 0.267, mul: 0.0007, add: 0.0007)
                ]
            )
        });
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
        }));

        // transmitir a fifo
        s.record(path: ~audio_pipe, numChannels: 2);

    }.play;
};
