
# info

Krug is simple program that tries to recognise chords (only major and minor for now).
You can see how much of each chord it recognised by brightness on circle of fifths and the strongest chord in the middle.
It isn't very accurate.  

![Image](image.webp)

# build

```sh
mkdir build
make release
```
run with ./exe

## dependencies

SDL2

### included:

[miniaudio](https://miniaud.io/)  
[fft](https://www.kurims.kyoto-u.ac.jp/~ooura/fft.html)

# config

configuration is made through simple config.txt file (just change the numbers)  
the file is read only at start

# how to capture audio from system (not microphone)

idk for windows but in linux I use pavucontrol and select "monitor of something" in input devices
(you need to select all input devices to see it)
You also might have to run `sudo modprobe snd-aloop` or something to enable loopback device
