s.options.hardwareBufferSize = 4096;
s.options.blockSize = 512;
s.options.sampleRate = 48000;
s.options.recHeaderFormat = "raw";
s.options.recSampleFormat = "int16";
// TODO hacer canales de salida 0
s.options.numOutputBusChannels = 2;
s.options.numInputBusChannels = 0;
s.boot;

thisProcess.openUDPPort(11301);

~mainPath       = PathName(thisProcess.nowExecutingPath).parentPath;
~projectPath    = PathName(~mainPath).parentPath;

(
    ~projectPath ++
    "audio-generator/script.scd"
).loadPaths;

(
    ~projectPath ++
    "video-generator-windowed/src/function_generator/function_generator.scd"
).loadPaths;

