# parrlib
C++/OpenGL wrapper developed by a student as a hobby

parrlib is a (kind-of) wrapper around most of the primary OpenGL functions, it is not complete by any means and probably somewhere (or everywhere) i'm doing things wrong;
i've been developing this wrapper since about 2015 with semi-long breaks inbetween years, it kind of grew itself as i kept on learning new functions to include in it;

parrlib is currently provided as a Visual Studio (2019) solution, in the future i plan to convert it to multi-platform but right now if you want to compile it on another OS, youll'have to delve into the code yourself and make changes accordingly.

parrlib focuses mainly on 2D apps, however there are already some 3D-oriented headers that could be helpful

currently documentation is pretty much non-existent (i only wrote comments when i felt like it), i'll try to document more functions in the future

if you want to compile a program using parrlib in its entirety, you'll need to compile (or find pre-compiled binaries of) the following external libraries:
  -GLFW alongside GLAD
  -SOIL                 --to load textures
  -freetype2            --mandatory, for text rendering
  -alut                 --if you use audio
  -assimp               --if you want to load 3d models made with external programs
  
i would provide include and library files myself (at least for Windows), but right now i don't know if i'm legally allowed to do so.
