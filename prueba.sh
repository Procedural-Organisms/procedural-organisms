#!/bin/bash

# matar todos los procesos y
# eliminar archivos con ctrl + c
# liberacion de recursos y eliminacion de archivos temporales
cleanup() {
    echo "Cleaning up..."
    rm -f "$audio_pipe" "$video_pipe"
    rm -f "$audio_ready" "$ffmpeg_started" "$video_started"
    kill 0 2>/dev/null
}
trap cleanup EXIT SIGINT

# localizacion de pipes
pipes_dir="temp/pipes"
audio_pipe="temp/pipes/audio_pipe"
video_pipe="temp/pipes/video_pipe"
# localizacion de flags
flags_dir="temp/flags"
audio_ready="temp/flags/audio_ready.flag"
ffmpeg_started="temp/flags/ffmpeg_started.flag"
video_started="temp/flags/video_started.flag"
# localizacion de renderer
video_generator="video-generator/build/video_generator"
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

# crear directorios temporales
if [ ! -d "$pipes_dir" ]; then
    mkdir -p "$pipes_dir"
fi
if [ ! -d "$flags_dir" ]; then
    mkdir -p "$flags_dir"
fi

# crear pipes
mkfifo "$audio_pipe"
mkfifo "$video_pipe"

# Iniciar servidor JACK solo en Linux
if [[ "$OSTYPE" != "darwin"* ]]; then
    jackd -d dummy -r 48000 -p 512 &
    sleep 2
fi

# inciciar script superCollider
# que inicia servidor y espera a que comience la generacion de video
sclang "$audio_generator" &

while [ ! -f "$audio_ready" ]; do
    sleep 0.1
done

# abrir generador de video en el background
# que espera a que ffmpeg comience para generar video
"$video_generator" > "$video_pipe" &

touch "$ffmpeg_started"

# generacion de video y conversion de vfr a cfr
ffmpeg \
-thread_queue_size 512 -f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i "$video_pipe" \
-thread_queue_size 512 -f s16le -ar 48000 -ac 2 -use_wallclock_as_timestamps 1 -i "$audio_pipe" \
-vf "vflip" -r 30 \
-c:v libvpx-vp9 -deadline realtime -cpu-used 8 -row-mt 1 -pix_fmt yuv420p -b:v 800k -g 30 \
-c:a libopus -ar 48000 -b:a 128k \
-f webm -cluster_size_limit 2M -content_type video/webm \
-listen 1 http://127.0.0.1:8080/stream.webm
