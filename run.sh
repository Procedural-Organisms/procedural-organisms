#!/bin/bash

# matar todos los procesos con ctrl + c
trap "kill 0" SIGINT
# eliminar archivos con ctrl + c
trap 'echo "Cleaning up..."; rm -f audio.wav output.mp4 audio_ready.flag video_started.flag; exit' SIGINT


# eliminar pipes
rm -f audio.wav
# eliminar archivos de prueba
rm -f output.mp4
# eliminar flags
rm -f audio_ready.flag
rm -f video_started.flag

# crear pipes
mkfifo audio.wav

# inciciar script superCollider
sclang audio-generator/script.scd &


while [ ! -f audio_ready.flag ]; do
    sleep 0.1
done

# generacion de video y conversion de vfr a cfr
video-generator/build/main | ffmpeg \
-f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i - \
-f s16le -ar 44100 -ac 2  -use_wallclock_as_timestamps 1 -i audio.wav \
-vf "vflip" \
-r 30 -c:v libx264 -c:a aac -pix_fmt yuv420p -shortest \
output.mp4
