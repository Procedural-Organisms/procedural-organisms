s.options.hardwareBufferSize = 2048;
s.options.blockSize = 512;
s.options.sampleRate = 44100;
s.options.recHeaderFormat = "raw";
s.options.recSampleFormat = "int16";
s.options.numOutputBusChannels = 0;
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
        {
            var lfo = SinOsc.kr(0.25, 0, 0.5, 0.5); // 2Hz LFO, range 0 to 1
            SinOsc.ar(110, 0, lfo).dup
        }.play;

        // transmitir a fifo
        s.record(path: ~audio_pipe, numChannels: 2);

    }.play;
};
