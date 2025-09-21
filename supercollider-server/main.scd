

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

