# QuakeViewer

A 3D Application that loads a BSP file and renders them using SDL2.

#### WORK IN PROGRESS!
The program is unfinished but now it can:

- Load PAK file from disk to memory
- Load BSP files

Future goals are:

- Set up camera and frustum class
- Create rendering layer
- Mark visibile areas using PVS (Partially Visible Set)
- Draw faces
- Frustum culling

The only dependencies used are SDL2 and glm, which are already provided in the "vendor" directory:
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [GLM](https://github.com/g-truc/glm)

Resources:

- [Quake 3.4 Unofficial Specs](https://www.gamers.org/dEngine/quake/spec/quake-spec34/qkmenu.htm)
- [Michael Abrash's Black Book](https://github.com/jagregory/abrash-black-book)
