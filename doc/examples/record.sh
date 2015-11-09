ffmpeg -r 60 -i %06d.png -f mov -c:a mp3 -c:v libx264 -pix_fmt yuv420p agent.mov
