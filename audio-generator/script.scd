s.options.hardwareBufferSize = 2048;
s.options.blockSize = 512;
s.options.sampleRate = 44100;
s.options.recHeaderFormat = "raw";
s.options.recSampleFormat = "int16";
s.options.numOutputBusChannels = 2;
s.options.numInputBusChannels = 0;
s.boot;

s.waitForBoot{
    // crear flag de inicializacion de servidor
    File("/Users/elektronischesstudio/Documents/01_proyectos/procedural-organisms/temp/flags/audio_ready.flag", "w").close;
    "Audio server ready. Waiting for video.".postln;

    // esperar a flag de inicializacion de video
    Routine{
        var flagPath = "/Users/elektronischesstudio/Documents/01_proyectos/procedural-organisms/temp/flags/video_started.flag";
        while {File.exists(flagPath).not}{0.1.wait};
        "Video started, starting audio and transmition.".postln;

        // inicializart de sintetizador
        { SinOsc.ar(220, 0, 0.2).dup }.play;

        // transmitir a fifo
        s.record(path: "/Users/elektronischesstudio/Documents/01_proyectos/procedural-organisms/temp/pipes/audio_pipe", numChannels: 2);

        25.wait;
        s.stopRecording;
        s.quit;
    }.play;
};
