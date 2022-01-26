# Vita-Mine-Sweeping
Mine sweeping for Psvita and Nintendo Switch with sdl2.

![avatar](https://github.com/baiXfeng/vita-mine-sweeping/assets/screenshot/screenshot1.png)
![avatar](https://github.com/baiXfeng/vita-mine-sweeping/assets/screenshot/screenshot2.png)

### Windows install depends:
- Install [MinGW](https://www.mingw-w64.org/downloads/)
- Install [SDL2-MinGw-dev](http://www.libsdl.org/download-2.0.php)
- Install [SDL2_image-MinGw-dev](http://www.libsdl.org/projects/SDL_image/)
- Install [SDL2_ttf-MinGw-dev](http://www.libsdl.org/projects/SDL_ttf/)
- Install [SDL2_mixer-MinGw-dev](http://www.libsdl.org/projects/SDL_mixer/)

### Build for Windows
- Install [CLion](https://www.jetbrains.com/clion/) - A cross-platform IDE for C and C++
- Run CLion and open vita-mine-sweeping folder
- Build and Run


### Linux install depends:
```
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev
```


### macOS install depends:
```
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx
```


### Build for Linux and macOS:
```
mkdir build && cd build
cmake ..
make
```

### Psvita install depends:
- Install [Vitasdk](https://vitasdk.org)

### Build for Psvita:
```
mkdir build && cd build
cmake -DPSV=ON ..
make
```

### Nintendo Switch install depends:
- Install [devkitPro](https://devkitpro.org/wiki/Getting_Started) and Install the following packages with devkitPro's [pacman](https://devkitpro.org/wiki/devkitPro_pacman):
```
switch-dev, switch-sdl2, switch-sdl2_gfx, switch-sdl2_image, switch-sdl2_ttf, switch-sdl2_mixer,
switch-freetype, switch-opusfile, switch-libopus, switch-mpg123, switch-libwebp, switch-libvorbisidec,
switch-libogg, switch-libpng, switch-libjpeg-turbo, switch-libmodplug, switch-flac, switch-glad,
switch-libdrm_nouveau, switch-zlib, switch-bzip2
```

### Build for Nintendo Switch:
```
mkdir build && cd build
cmake -DNS=ON ..
make
```
