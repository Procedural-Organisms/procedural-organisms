#!/bin/bash

# eliminacion de video de muestra
rm -f output.mp4

# generacion de video y conversion de vfr a cfr
video-generator/build/main | ffmpeg \
-f rawvideo -pix_fmt rgba -video_size 400x300 -use_wallclock_as_timestamps 1 -i - \
-vf "vflip" \
-r 30 -fps_mode cfr -pix_fmt yuv420p \
output.mp4
