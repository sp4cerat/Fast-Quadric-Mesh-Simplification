# Fast-Quadric-Mesh-Simplification
Mesh triangle reduction using quadrics

![img](https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification/blob/master/screenshot.png?raw=true)

**Summary** Since I couldnt find any code thats fast, memory efficient, free and for high quality, I developed my version of the quadric based edge collapse mesh simplification method. It uses a threshold to determine which triangles to delete, which avoids sorting but might lead to lesser quality. It is about 4x as fast as Meshlab and can simplify 2M -> 30k triangles in 3.5 seconds.

**Usage** The functionality is contained in Simplify.h. The function to call is *simplify_mesh(target_count)*. The code is kept pretty slim, so the main method has just around 400 lines of code. 

**Obj File Limitations** The Obj file may only have one group or object. Its a very simple reader/writer, so dont try to use multiple objects in one file

License : MIT

Update May 11th 2016:

**OSX and Linux Version + Command Line Tool added**

[Chris Rorden](https://github.com/neurolabusc) created a command line version and provided binaries for OSX and Linux.

**Pascal Version**

[Chris Rorden](https://github.com/neurolabusc) also created a pascal version of the tool:

https://github.com/neurolabusc/Fast-Quadric-Mesh-Simplification-Pascal-



Please dont forget to cite this page if you use the code!
