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
output="rtmp://a.rtmp.youtube.com/live2/caee-596b-76td-tv9k-8g21"

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

jackd -d alsa -r 44100 -p 4096 -n 3 -S &

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
while ! lsof "$video_pipe" | grep -q ffmpeg; do
    sleep 0.1
done
touch "$ffmpeg_started"
} &

# generacion de video y conversion de vfr a cfr
ffmpeg \
-thread_queue_size 4096 -f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i "$video_pipe" \
-thread_queue_size 4096 -f s16le -ar 44100 -ac 2 -i "$audio_pipe" \
-vf "vflip" \
-r 30 -f flv -c:v libx264 -pix_fmt yuv420p -b:v 400k -c:a aac -ar 44100 -b:a 128k "$output"

# eliminar pipes
rm -f "$audio_pipe"
rm -f "$video_pipe"
# eliminar flags
rm -f "$audio_ready"
rm -f "$ffmpeg_started"
rm -f "$video_started"
