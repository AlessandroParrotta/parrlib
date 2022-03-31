# parrlib
C++/Windows exclusive framework that facilitates creating and developing graphics applications.

parrlib builds a layer on top of existing graphics APIs and abstracts them into simpler and shorter functions that can be called in order to draw things on screen.

currently, parrlib supports:
  * OpenGL
  * DX11
  * software renderer (incomplete)

parrlib uses a structure of backends to communicate with graphics APIs.
backends are .dll files used to interface with the target graphics driver; a specific backend is loaded at startup dinamically based on user request.

# Build order
before building any of the other backends, it is necessary to build 'parrlib' first, and generate an object file since every backend eventually links to 'parrlib.lib'.

# External Requirements
in order to be fully built, parrlib requires:
  * [DirectX Software Development Kit](https://www.microsoft.com/en-us/download/details.aspx?id=6812) 
  * [freetype2](https://freetype.org/) (at least version 2.11.10)
  * [SOIL](https://github.com/littlstar/soil)
	
[assimp](https://github.com/assimp/assimp) is used in the '3D' sets of folders inside 'parrlib/math' ('utils3d' and 'physics3d'), however, after a refactor of the whole codebase, the classes in this folder don't work anymore (eventually it'll be fixed), so try not to include any of the .h files in 'parrlib/math/utils3d' and 'parrlib/math/physics3d' for now.

# Examples
if you want to get started with parrlib, you can visit the repository [examples](https://github.com/AlessandroParrotta/parrlib-examples) which contains some simple code samples.
