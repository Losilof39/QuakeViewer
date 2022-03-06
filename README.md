# QuakeViewer

A 3D Application that loads a BSP file and renders them using SDL2.

#### WORK IN PROGRESS!
The program is unfinished but now it can:

- Load PAK file from disk to memory
- Load BSP files
- Draw basic shapes
- Render 3D objects
- Backface culling
- Load 3D model from obj files

Future goals are:

- Set up camera and frustum class
- Texture mapping (with perspective correction)
- Create rendering layer
- Mark visibile areas using PVS (Partially Visible Set)
- Draw faces
- Frustum culling
- Clipping against the screen edges

The only library used is SDL2, which is already provided in the "vendor" directory:
- [SDL2](https://www.libsdl.org/download-2.0.php)

Resources:

- [Quake 3.4 Unofficial Specs](https://www.gamers.org/dEngine/quake/spec/quake-spec34/qkmenu.htm)
- [Michael Abrash's Black Book](https://github.com/jagregory/abrash-black-book)
