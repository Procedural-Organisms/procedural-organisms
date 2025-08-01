#!/bin/bash

trap "kill 0" SIGINT

# eliminacion de pipes
rm -f audio.wav
rm -f video.yuv

# eliminacion de video de muestra
rm -f output.mp4

# creacion de named pipe para video con cfr

# creacion de named pipe para audio
mkfifo audio.wav
mkfifo video.yuv

# generacion de video y conversion de vfr a cfr
video-generator/build/main | ffmpeg \
-f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i - \
-vf "vflip" \
-fps_mode cfr -r 30 -pix_fmt yuv420p -f rawvideo \
-y video.yuv &

# captura de video con cfr y audio
ffmpeg -f rawvideo -pix_fmt yuv420p -video_size 400x300 -i video.yuv \
-f s16le -ar 44100 -ac 2 -i audio.wav \
-r 30 -c:v libx264 -c:a aac -pix_fmt yuv420p -shortest \
output.mp4


# eliminacion de pipes
rm audio.wav
rm video.yuv


# -f flv -r 30 -pix_fmt yuv420p -shortest \ 
# rtmp://a.rtmp.youtube.com/live2/caee-596b-76td-tv9k-8g21
