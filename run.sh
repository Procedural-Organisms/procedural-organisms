#!/bin/bash

trap "kill 0" SIGINT

mkfifo audio.wav

# eliminacion de video de muestra
rm -f output.mp4

# generacion de video y conversion de vfr a cfr
video-generator/build/main | ffmpeg \
-f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i - \
-f s16le -ar 44100 -ac 2 -i audio.wav \
-vf "vflip" \
-r 30 -c:v libx264 -c:a aac -pix_fmt yuv420p -shortest \
output.mp4 &

sclang audio-generator/synth.scd 

rm -f audio.wav
