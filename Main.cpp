// Simple wrapper for Sven Forstmann's mesh simplification tool
//
// Loads a OBJ format mesh, decimates mesh, saves decimated mesh as OBJ format
// http://voxels.blogspot.com/2014/05/quadric-mesh-simplification-with-source.html
// https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification
//To compile for Linux/OSX (GCC/LLVM)
//  g++ Main.cpp -O3 -o simplify
//To compile for Windows (Visual Studio)
// vcvarsall amd64
// cl /EHsc Main.cpp /osimplify
//To execute
//  ./simplify wall.obj out.obj 0.04
//
// Pascal Version by Chris Roden:
// https://github.com/neurolabusc/Fast-Quadric-Mesh-Simplification-Pascal-
//

#include "Simplify.h"
#include <stdio.h>
#include <time.h>  // clock_t, clock, CLOCKS_PER_SEC

void showHelp(const char * argv[]) {
    const char *cstr = (argv[0]);
    printf("Usage: %s <input> <output> <ratio> <agressiveness)\n", cstr);
    printf(" Input: name of existing OBJ format mesh\n");
    printf(" Output: name for decimated OBJ format mesh\n");
    printf(" Ratio: (default = 0.5) for example 0.2 will decimate 80%% of triangles\n");
    printf(" Agressiveness: (default = 7.0) faster or better decimation\n");
    printf("Examples :\n");
#if defined(_WIN64) || defined(_WIN32)
    printf("  %s c:\\dir\\in.obj c:\\dir\\out.obj 0.2\n", cstr);
#else
    printf("  %s ~/dir/in.obj ~/dir/out.obj 0.2\n", cstr);
#endif
} //showHelp()

int simplify(std::string file_path, std::string export_path, float reduceFraction, float agressiveness) {

    printf("Mesh Simplification (C)2014 by Sven Forstmann in 2014, MIT License (%zu-bit)\n", sizeof(size_t)*8);

    if (file_path.empty() or export_path.empty()) {
        printf("file path or export path is empty!");
        return EXIT_SUCCESS;
    }

    std::string file_extension = file_path.substr(file_path.find_last_of('.')+1, file_path.size());
    printf("file extension is %s\n", file_extension.c_str());

    if (file_extension == "obj") {
        Simplify::load_obj(file_path.c_str());
        printf("loading obj\n");
    }
    else if (file_extension == "stl") {
        printf("loading stl\n");
        Simplify::load_stl(file_path.c_str());
    } else {
        printf("file is not obj or stl %s with extension %s\n", file_path.c_str(), file_extension.c_str());
        return EXIT_FAILURE;
    }

    if ((Simplify::triangles.size() < 3) || (Simplify::vertices.size() < 3)) {
        printf("triangles size %d vertices size %d\n", Simplify::triangles.size(), Simplify::vertices.size());
        return EXIT_FAILURE;
    }

    int target_count =  Simplify::triangles.size() >> 1;

    if (reduceFraction > 1.0) reduceFraction = 1.0; //lossless only
    if (reduceFraction <= 0.0) {
        printf("Ratio must be BETWEEN zero and one.\n");
        return EXIT_FAILURE;
    }
    target_count = round((float)Simplify::triangles.size() * reduceFraction);

    if (target_count < 4) {
        printf("Object will not survive such extreme decimation\n");
        return EXIT_FAILURE;
    }
    clock_t start = clock();
    printf("Input: %zu vertices, %zu triangles (target %d)\n", Simplify::vertices.size(), Simplify::triangles.size(), target_count);
    int startSize = Simplify::triangles.size();
    Simplify::simplify_mesh(target_count, agressiveness, true);
    //Simplify::simplify_mesh_lossless( false);
    if ( Simplify::triangles.size() >= startSize) {
        printf("Unable to reduce mesh.\n");
        return EXIT_FAILURE;
    }

    std::string export_extension = export_path.substr(export_path.find_last_of(".")+1);
    if (export_extension == "obj") {
        printf("exporting obj\n");
        Simplify::write_obj(export_path.c_str());
    }
    else if (export_extension == "stl") {
        printf("exporting stl\n");
        Simplify::write_stl(export_path.c_str());
    } else {
        printf("export file is not obj or stl %s\n", export_path.c_str());
        return EXIT_FAILURE;
    }
    printf("Output: %zu vertices, %zu triangles (%f reduction; %.4f sec)\n",Simplify::vertices.size(), Simplify::triangles.size()
        , (float)Simplify::triangles.size()/ (float) startSize  , ((float)(clock()-start))/CLOCKS_PER_SEC );
    return EXIT_SUCCESS;
}

extern "C" {
int simplify(std::string file_path, float reduceFraction) {
    printf("Going to simplify %s\n", file_path.c_str());
    return simplify(file_path, "simplify.stl", reduceFraction, 0.7);// aggressive
}
}

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        showHelp(argv);
        return EXIT_SUCCESS;
    }

    std::string file_path = argv[1];
    std::string export_path = argv[2];
    float reduceFraction = 0.5;
    if (argc > 3) {
        reduceFraction = atof(argv[3]);
    }

    float agressiveness = 7.0;
    if (argc > 4) {
        agressiveness = atof(argv[4]);
    }
    return simplify(file_path, export_path, reduceFraction, agressiveness);
}
