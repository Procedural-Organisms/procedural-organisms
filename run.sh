#!/bin/bash

# matar todos los procesos y
# eliminar archivos con ctrl + c
trap 'echo "Cleaning up..."; kill 0; rm -f audio.wav output.mp4 audio_ready.flag video_started.flag; exit' SIGINT

# localizacion de pipes
audio_pipe="temp/pipes/audio_pipe"
video_pipe="temp/pipes/video_pipe"
# localizacion de flags
audio_ready="temp/flags/audio_ready.flag"
ffmpeg_started="temp/flags/ffmpeg_started.flag"
video_started="temp/flags/video_started.flag"
# localizacion de renderer
video_generator="video-generator/build/main"
# lacalizacion de sintetizador
audio_generator="audio-generator/script.scd"
# localizacion de output
output="test/output.mp4"

# eliminar pipes
rm -f "$audio_pipe"
rm -f "$video_pipe"
# eliminar archivos de prueba
rm -f "$output"
# eliminar flags
rm -f "$audio_ready"
rm -f "$ffmpeg_started"
rm -f "$video_started"

# crear pipes
mkfifo "$audio_pipe"
mkfifo "$video_pipe"

# inciciar script superCollider
# que inicia servidor y espera a que comience la generacion de video
sclang "$audio_generator" &

while [ ! -f "$audio_ready" ]; do
    sleep 0.1
done

# abrir generador de video en el background
# que espera a que ffmpeg comience para generar video
"$video_generator" > "$video_pipe" &

# esperar en el background a que se abra ffmpeg y crear flag
{
while !lsof "$video_pipe" | grep -q ffmpeg; do
    sleep 0.1
done
sleep 5
touch "$ffmpeg_started"
} &

# generacion de video y conversion de vfr a cfr
ffmpeg \
-f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i "$video_pipe" \
-f s16le -ar 44100 -ac 2 -i "$audio_pipe" \
-vf "vflip" \
-r 30 -c:v libx264 -c:a aac -pix_fmt yuv420p -t 20 "$output"


