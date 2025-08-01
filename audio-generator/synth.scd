s.options.hardwareBufferSize = 2048;
s.options.blockSize = 512;
s.options.sampleRate = 44100;
s.options.recHeaderFormat = "wav";
s.options.recSampleFormat = "int16";
s.options.numOutputBusChannels = 2;
s.options.numInputBusChannels = 0;
s.boot;

s.waitForBoot{
    if (s.isRecording) { s.stopRecording; };

    { SinOsc.ar(220, 0, 0.2).dup }.play;

    s.record(path: "/Users/elektronischesstudio/Documents/01_proyectos/procedural-organisms/audio.wav", numChannels: 2);

    Routine {
        30.wait;  // record for 10 seconds
        s.stopRecording;
        s.quit;
    }.play;
}




