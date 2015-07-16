# Fast-Quadric-Mesh-Simplification
Mesh triangle reduction using quadrics

**Summary** Since I couldnt find any code thats fast, memory efficient, free and for high quality, I developed my version of the quadric based edge collapse mesh simplification method. It uses a threshold to determine which triangles to delete, which avoids sorting but might lead to lesser quality. It is about 4x as fast as Meshlab and can simplify 2M -> 30k triangles in 3.5 seconds. Here a comparison of the [results](http://4.bp.blogspot.com/-Vqb8ac6aHPc/U3I-tDop4LI/AAAAAAAAC4g/xxaBFOSq6so/s1600/Clipboard03.png) (The fast method left and the Meshlab result right).

**Alternative Download** [You can fetch the code here](http://voxels.blogspot.jp/2014/05/quadric-mesh-simplification-with-source.html)

**Usage** The functionality is contained in Simplify.h. The function to call is *simplify_mesh(target_count)*. The code is kept pretty slim, so the main method has just around 400 lines of code. 
