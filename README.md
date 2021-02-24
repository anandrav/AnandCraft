# AnandCraft
A Minecraft clone made by Anand Dukkipati

## Feature Todo List
- [x] blocks w/ textures
- [x] chunks
- [x] breaking and placing of blocks
- [x] saving and loading terrain from files
- [x] multi-threaded terrain (loading/saving, mesh building)
- [ ] basic UI/Menu system (crosshair, block selection, world selection)

## Build on macOS
```
brew install cmake sdl2 glew glm
cmake .
make
./AnandCraft
```

## Build on Debian/Ubuntu
```
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt-get install glew-utils
sudo apt install libglm-dev
cmake .
make
./AnandCraft
```

## Screenshots:
<img src="screenshots/grass-block.jpg" width="500">
<img src="screenshots/glass-and-cobble-chunk.jpg" width="500">
<img src="screenshots/castle.jpg" width="500">
