# parrlib
C++/Windows exclusive framework that facilitates creating and developing graphics applications.

parrlib builds a layer on top of existing graphics APIs and abstracts them into simpler and shorter functions that can be called in order to draw things on screen.

currently, parrlib supports:
  * OpenGL
  * DX11
  * software renderer (incomplete)

parrlib uses a structure of backends to communicate with graphics APIs.
backends are .dll files used to interface with the target graphics driver; a specific backend is loaded at startup dinamically based on user request.

# parrlibcore
[parrlibcore](https://github.com/AlessandroParrotta/parrlibcore) is the core module used in parrlib to manage common objects (like matrices, vectors, etc..) between all parrlib libraries; in order to build parrlib, you'll need to build or download a pre-built binary of parrlibcore beforehand.

# Build order
before building any of the other backends, it is necessary to build 'parrlib' first, and generate an object file since every backend eventually links to 'parrlib.lib'.

# External Requirements
in order to be fully built (backends included), parrlib requires:
  * [parrlibcore](https://github.com/AlessandroParrotta/parrlibcore)
  * [DirectX Software Development Kit](https://www.microsoft.com/en-us/download/details.aspx?id=6812) 
  * [SOIL](https://github.com/littlstar/soil)
  * [freetype2](https://freetype.org/) (at least version 2.11.10)
  * [freealut](https://github.com/vancegroup/freealut)
  * [assimp](https://github.com/assimp/assimp) 

I'll provide two versions for every release of parrlib, a minimal one (named -min) that contains only the essential binaries and includes that represent parrlib, and a full one (named -full) that contains all include and pre-built binaries of all the aforementioned libraries (EXCEPT for the DirectX Software Development Kit which you'll need to install into your system) (however, the versions will probably need to be updated if you want all latest features).

# User Macros for include/library paths
parrlib uses some User Macros to facilitate the development of multiple projects at the same time; 
A file called PropertySheet.props that contains the names of these macros (with empty value fields) is included in the root folder of the project; it is responsibility of the user to populate the macros of this PropertySheet in order to be able to include the header and library files within the project (alternatively, it is possible to just edit the Include and Library directories in the project properties)

# Examples
if you want to get started with parrlib, you can visit the repository [examples](https://github.com/AlessandroParrotta/parrlib-examples) which contains some simple code samples.
