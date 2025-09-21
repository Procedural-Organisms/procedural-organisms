~audio_ready = "temp/flags/audio_ready.flag";
~video_started = "temp/flags/video_started.flag";
~audio_pipe = "temp/pipes/audio_pipe";

~mainPath       = PathName(thisProcess.nowExecutingPath).parentPath;
~projectPath    = PathName(~mainPath).parentPath;

(
    ~projectPath ++
    "audio-generator/script.scd"
).loadPaths;

s.waitForBoot{
    // crear flag de inicializacion de servidor
    File(~audio_ready, "w").close;

    "Audio server ready. Waiting for video.".postln;
    ~receiver = NetAddr("localhost", 13933);    
    if(
        thisProcess.openUDPPort(13933) == true,
        postln(
            "SuperCollider OSC server ready!\nServer address: " ++
            ~receiver.ip ++ ":" ++ ~receiver.port
        )
    );

    (
        ~projectPath ++
        "video-generator-windowed/src/function_generator/function_generator.scd"
    ).loadPaths;
}
