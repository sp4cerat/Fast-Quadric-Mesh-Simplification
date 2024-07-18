# Fast-Quadric-Mesh-Simplification
Mesh triangle reduction using quadrics - for Windows, OSX and Linux (thx Chris Rorden)

![img](https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification/blob/master/screenshot.png?raw=true)

**Summary** Since I couldn't find any code that's fast, memory efficient, free and for high quality, I developed my version of the quadric based edge collapse mesh simplification method. It uses a threshold to determine which triangles to delete, which avoids sorting but might lead to lesser quality. It is about 4x as fast as Meshlab and can simplify 2M -> 30k triangles in 3.5 seconds.

**Usage** The functionality is contained in Simplify.h. The function to call is *simplify_mesh(target_count)*. The code is kept pretty slim, so the main method has just around 400 lines of code.

** Compiling and Usage **

You can compile the command line program from the `src.cmd` folder. This simple program simplifies meshes stored in [OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) format. This is a simple reader/writer, so don't try to use this tool when multiple objects are embedded into one file.

```
g++ Main.cpp -O3 -o simplify
./simplify ~/dir/in.obj ~/dir/out.obj 0.2
```

**Translations, Implementations and Live Demos**

 - [C++](https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification) (this page).
 - [C](https://github.com/neurolabusc/nii2mesh) translation with Marching Cubes.
 - [C# for Unity mesh simplification](https://github.com/Whinarn/UnityMeshSimplifier).
 - [C# for .NET mesh simplification](https://github.com/Whinarn/MeshDecimator).
 - [Java mesh simplifier by Jayfella](https://hub.jmonkeyengine.org/t/isosurface-mesh-simplifier/41046).
 - JavaScript [ThreeJS](https://github.com/timknip/mesh-decimate) for native [JavaScript](https://gist.github.com/zz85/a317597912d68cf046558006d7647381) and Web Assembly implementations, illustrated with a [live demo web page](https://neurolabusc.github.io/simplifyjs/).
 - JavaScript [NiiVue](https://github.com/niivue/niivue-mesh) for converting voxels to meshes and simplifying meshes with [live demo web page](https://niivue.github.io/niivue-mesh/).
 - [JavaScript translation](https://github.com/ataber/mesh-simplify) available from [npm](https://www.npmjs.com/package/mesh-simplify).
 - [JavaScript WebAssembly](https://github.com/MyMiniFactory/Fast-Quadric-Mesh-Simplification) port with [live demo](https://myminifactory.github.io/Fast-Quadric-Mesh-Simplification/).
 - [Pascal](https://github.com/neurolabusc).
 - Pascal [Surf-Ice](https://www.nitrc.org/plugins/mwiki/index.php/surfice:MainPage) desktop application for Windows, Linux and MacOS.