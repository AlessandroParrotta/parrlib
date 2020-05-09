# parrlib
C++/OpenGL wrapper developed by a student as a hobby

parrlib is a (kind-of) wrapper around most of the primary OpenGL functions, it is not complete by any means and probably somewhere (or everywhere) i'm doing things wrong;
i've been developing this wrapper since about 2015 with semi-long breaks inbetween years, it kind of grew itself as i kept on learning new functions to include in it;

whilst being set up as a CMake project, parrlib is still not compatible with OSes other than Windows

parrlib focuses mainly on 2D apps, however there are already some 3D-oriented headers that could be helpful

currently documentation is pretty much non-existent (i only wrote comments when i felt like it), i'll try to document more functions in the future

if you want to compile a program using parrlib in its entirety, you'll need to compile (or find pre-compiled binaries of) the following external libraries:
  * GLFW3
  * SOIL                 --to load textures
  * freetype2            --mandatory, for text rendering
  * alut                 --if you use audio
  * assimp               --if you want to load 3d models made with external programs
  
external include files have already been provided to guarantee the compilation of parrlib (in the thirdparty/ folder), i would provide library files myself (at least for Windows), but right now i don't know if i'm legally allowed to do so.

# Pre-compiled binary (Windows)
[here](https://mega.nz/file/FqRQyQCL#SAyBrLUl3MbqVdp10cEThBnv2ad1cr1xmgA8HUGC8z0) is a pre-compiled binary of parrlib, keep in mind it might not be updated to the latest commit

# Examples
if you want to get started with parrlib, i suggest taking a look at my other repository [examples](https://github.com/AlessandroParrotta/parrlib-examples)

i suggest reading the README file there as the examples are not set up for compilation
