
# info

Krug is simple program that tries to recognise chords (only major and minor for now).
You can see how much of each chord it recognised by brightness on circle of fifths and the strongest chord in the middle.
It isn't very accurate.

# build

```sh
mkdir build
make release
```

## dependencies

SDL2

### included:

[miniaudio](https://miniaud.io/)  
[fft](https://www.kurims.kyoto-u.ac.jp/~ooura/fft.html)

# config

configuration is made through simple config.txt file (just change the numbers)  
the file is read only at start
