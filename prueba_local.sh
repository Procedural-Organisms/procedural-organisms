#!/bin/bash

# matar todos los procesos y
# eliminar archivos con ctrl + c
# liberacion de recursos y eliminacion de archivos temporales
cleanup() {
    echo "Cleaning up..."
    rm -f "$audio_ready" "$ffmpeg_started" "$video_started"
    kill 0 2>/dev/null
}
trap cleanup EXIT SIGINT

# localizacion de flags
flags_dir="temp/flags"
audio_ready="temp/flags/audio_ready.flag"
ffmpeg_started="temp/flags/ffmpeg_started.flag"
video_started="temp/flags/video_started.flag"
# localizacion de renderer
video_generator="video-generator-local/src/build/main"
# lacalizacion de sintetizador
audio_generator="audio-generator/script.scd"

# eliminar flags
rm -f "$audio_ready"
rm -f "$ffmpeg_started"
rm -f "$video_started"

# crear directorios temporales
if [ ! -d "$flags_dir" ]; then
    mkdir -p "$flags_dir"
fi

# inciciar script superCollider
# que inicia servidor y espera a que comience la generacion de video
sclang "$audio_generator" &

while [ ! -f "$audio_ready" ]; do
    sleep 0.1
done

touch "$ffmpeg_started"

# abrir generador de video en el background
# que espera a que ffmpeg comience para generar video
"$video_generator"

