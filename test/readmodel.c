#include <stdio.h>
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

typedef struct PointPair{
    unsigned int StartPointIndex;
    unsigned int EndPointIndex;
} PointPair;

int main(){
    PointPair* p;
    unsigned int PosIndex = 0;

    fastObjMesh *mesh = fast_obj_read("../assest/Cube.obj");

    if(!mesh){
	   printf("ERROR");
	   return -1;
    }

    p = (PointPair*)malloc(mesh->index_count * sizeof(PointPair));
    // p = (PointPair*)malloc(sizeof(unsigned int) * );

    printf("face count: %d\n", mesh->face_count);
    printf("index count: %d\n", mesh->index_count);
    printf("position count: %d\n", mesh->position_count);

    unsigned int posIndex = 0;
    for(int i = 0; i < mesh->face_count; i++){
	   // printf("face vertices count: %d\n", mesh->face_vertices[i]);
	   for(int j = 0; j < mesh->face_vertices[i]; j++){
		// printf("Point Index is %d ",mesh->indices[PosIndex + j]);
		//   printf("Point Par is [%d, %d]\n", 
		// 		mesh->indices[PosIndex + j].p, 
		// 		mesh->indices[PosIndex + (j + 1 < mesh->face_vertices[i] ? j + 1 : 0)].p
		// 		);
		  p[PosIndex + j].StartPointIndex = mesh->indices[PosIndex + j].p;
		  p[PosIndex + j].EndPointIndex = mesh->indices[PosIndex + (j + 1 < mesh->face_vertices[i] ? j + 1 : 0)].p;
		  // printf("Point Par is [%d, %d]\n", p[PosIndex + j].StartPointIndex, p[PosIndex + j].EndPointIndex);
		  printf("[%d, %d] ", p[PosIndex + j].StartPointIndex, p[PosIndex + j].EndPointIndex);

	   }
	   printf("\n");
	   PosIndex += mesh->face_vertices[i];
    }
    printf("PosIndex is  %d\n", PosIndex);

    // for(int i = 0; i < 24; i++) {
    //     printf("%02d: %d %d\n", i, p[i].StartPointIndex, p[i].EndPointIndex);
    // }

    free(p);
    fast_obj_destroy(mesh);
    return 0;
}
