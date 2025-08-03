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

pkill -f ffmpeg
pkill -f supercollider
killall jackd
