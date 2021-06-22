## 42 - Scop

### Goal
> - Your goal is to create a small program that will show a 3D object stored in a .obj file. You will be at least in charge of parsing to obtain the requested rendering.
> - In a window, your 3D object will be displayed in perspective, rotate on itself around its main symmetrical axis. 
> - By using various colors, it must be possible to distinguish the various sides. The object can be moved on three axis, in both directions.
> - A texture must be applicable simply on the object when we press a dedicated
key, and the same key allows to go back to the different colors. A soft transition between the two is requested.

### Constraints
- C Language
- Modern OpenGL (min v4)
- Shaders
- No external library other than libc and libm, except for Window and Events handling

------------
Here's the list of whats working and what not.

**Bold** elements are mandatory.

## Whats working
- **Shader parsing and building**
- **Object parsing** (**vertices**, texture coords, normals and **face**)
- **Face format** (**v**, v/vt, v//vn, v/vt/vn)
- **Object display**
- **Rotation around object center**
- **Object translation**
- **Texture display** (with UV mapping)
- **Texture display** ( **without** UV mapping )
- Camera transformation
- Model transformation
- Polygon triangulation
- Multiple objects
- MTL File parsing
- Texture file parsing
- Generating missing normals
- Keybinding to switch between multiple rendering mode
- Mouse camera rotation
- Keyboard camera translation
- Directional light
- Automatic multi-object positioning
