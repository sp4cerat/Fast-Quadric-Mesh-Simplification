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
// WebAssembly Port by MyMiniFactory
//  https://github.com/MyMiniFactory/Fast-Quadric-Mesh-Simplification
// To compile for WebAssembly
//  make wasm

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

bool is_extension(const char* file_path, const char* extension) {
	char file_extension[3];
	file_extension[0] = file_path[strlen(file_path)-3];
	file_extension[1] = file_path[strlen(file_path)-2];
	file_extension[2] = file_path[strlen(file_path)-1];
	return (file_extension[0] == extension[0]
		and file_extension[1] == extension[1]
		and file_extension[2] == extension[2]);
}

bool is_obj(const char* file_path) {
	return is_extension(file_path, "obj");
}

bool is_mz3(const char* file_path) {
	return is_extension(file_path, "mz3");
}

int simplify(const char* file_path, const char* export_path, float reduceFraction, float agressiveness) {

	printf("Mesh Simplification (C)2014 by Sven Forstmann in 2014, MIT License (%zu-bit)\n", sizeof(size_t)*8);

	if (is_obj(file_path)) {
		Simplify::load_obj(file_path, true);
		printf("loading obj\n");
	}
	else if (is_mz3(file_path)) {
		printf("loading mz3\n");
		Simplify::load_mz3(file_path);
	} else {
		printf("file is not obj or mz3 %s\n", file_path);
		return EXIT_FAILURE;
	}

	if ((Simplify::triangles.size() < 3) || (Simplify::vertices.size() < 3)) {
		printf("triangles size or vertices size less than 3\n");
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

	if (is_obj(export_path)) {
		printf("exporting obj\n");
		Simplify::write_obj(export_path);
	}
	else if (is_mz3(export_path)) {
		printf("exporting mz3\n");
		Simplify::write_mz3(export_path);
	} else {
		printf("export file is not obj or stl %s\n", export_path);
		return EXIT_FAILURE;
	}

	printf("Output: %zu vertices, %zu triangles (%f reduction; %.4f sec)\n",Simplify::vertices.size(), Simplify::triangles.size()
		, (float)Simplify::triangles.size()/ (float) startSize  , ((float)(clock()-start))/CLOCKS_PER_SEC );
	return EXIT_SUCCESS;
}

#ifdef __EMSCRIPTEN__

extern "C" {
int simplify(const char* file_path, float reduceFraction, const char* export_path, float agressiveness) {
	printf("Going to simplify %s\n", file_path);
	return simplify(file_path, export_path, reduceFraction, agressiveness);
}
}

#else

int main(int argc, const char * argv[]) {
	if (argc < 3) {
		showHelp(argv);
		return EXIT_SUCCESS;
	}
	
	const char* file_path = argv[1];
	const char* export_path = argv[2];
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

#endif