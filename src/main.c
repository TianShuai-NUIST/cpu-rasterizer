#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

const int width = 600;
const int height = 600;

#define BACKGROUND BLACK
#define FOREGROUND GREEN

typedef struct PointPair {
    unsigned int StartPointIndex;
    unsigned int EndPointIndex;
} PointPair;

void Point(Vector2 pos, float r){
    DrawRectangle(pos.x-r/2,pos.y-r/2, r, r, FOREGROUND);                        // Draw a color-filled rectangle
}

// 需要清楚我们需要做什么
// 点 -> 动画 -> 变换2D -> 坐标系映射 -> 渲染 


// 把空间中的点映射到平面上 
Vector2 Mapping(Vector3 pos){
    return (Vector2){
	   pos.x/pos.z,
	   pos.y/pos.z
    };
}

// 中心坐标系变换屏幕坐标系
Vector2 Screen(Vector2 pos){
    // -1..1 => 0..2 => 0..1 => 0...max
    return (Vector2){
	   (pos.x + 1) / 2.0f * width,
	   (1 - (pos.y + 1) / 2.0f) * height
    };
}

// 变换矢量
Vector3 TransformV(Vector3 pos, Vector3 delta){
    return (Vector3){
	   pos.x + delta.x,
	   pos.y + delta.y,
	   pos.z + delta.z
    };
}

void LinkVertex(Vector2* vb,PointPair* ib,unsigned int length){
    for(int i = 0; i < length; i++){
	   if((ib+i)->StartPointIndex<0 || (ib+i)->EndPointIndex<0){
		  continue;
	   } else {
		  DrawLineV(vb[(int)(ib+i)->StartPointIndex],vb[(int)(ib+i)->EndPointIndex],FOREGROUND);
	   }
    }
}

Vector3 RotationY(float angle, Vector3 point, Vector3 center){
    Vector3 result;
    result.x = (point.x-center.x)*cosf(angle) - (point.z-center.z)*sinf(angle);
    result.y = point.y;
    result.z = (point.x-center.x)*sinf(angle) + (point.z-center.z)*cosf(angle);

    return result;
}

Vector3 FaceCenter(Vector3 start,Vector3 end){
    Vector3 result;

    result.x = (start.x+end.x)/2.0f;
    result.y = (start.y+end.y)/2.0f;
    result.z = (start.z+end.z)/2.0f;

    return result;
}

int main(int argc, char **argv){

    char* model_path;

    if(argc < 2){
	   printf("Argument to less...\nUsage Page is %s <ModelPath>\n",argv[0]);
	   exit(1); 
    } else {
	   model_path = (char*)malloc(sizeof(char) * (strlen(argv[1] + 1)));
	   strcpy(model_path, argv[1]);
    }

    float angle = 0.0f; 
    float dz = 5.0f;
    Vector3 *vb = NULL;
    Vector3 center;
    Vector2 *rvb = NULL;
    PointPair *ib = NULL;

    fastObjMesh* mesh = fast_obj_read(model_path);
    // fastObjMesh* mesh = fast_obj_read("assest/Cube.obj");

    if(!mesh){
	   perror("Read Obj error");
	   return -1; 
    }

    // Index Buffer
    ib = (PointPair*)malloc(mesh->index_count * sizeof(PointPair));
    vb = (Vector3 *)malloc(mesh->position_count * sizeof(Vector3));
    rvb = (Vector2 *)malloc(mesh->position_count * sizeof(Vector2));

    // Read Position of point 1..position_count 
    // Actully, Position Count is more than the Position Count of Obj, beacause the index of zero is a flag
    unsigned int PositionsCount = mesh->position_count;
    for (int i = 1; i < PositionsCount; i++){
	   vb[i].x = mesh->positions[i * 3 + 0];
	   vb[i].y = mesh->positions[i * 3 + 1];
	   vb[i].z = mesh->positions[i * 3 + 2];
	   // printf("%d is (%.2f, %.2f, %.2f)\n", i, vb[i].x, vb[i].y, vb[i].z);
    }

    // Read Index of Line
    unsigned int PosIndex = 0;
    unsigned int LineCount = 0;
    for(int i = 0; i < mesh->face_count; i++){
	   for(int j = 0; j < mesh->face_vertices[i]; j++){
		  ib[PosIndex + j].StartPointIndex = mesh->indices[PosIndex + j].p;
		  // Loop List, set tail to head
		  ib[PosIndex + j].EndPointIndex = mesh->indices[PosIndex + (j + 1 < mesh->face_vertices[i] ? j + 1 : 0)].p;
		  // printf("[%d %d]",ib[PosIndex + j].StartPointIndex, ib[PosIndex + j].EndPointIndex);
		  LineCount++;
	   }
	   // printf("\n");
	   PosIndex += mesh->face_vertices[i];
    }
    // printf("Sum of Point is %d\n", PosIndex);

    // center = { (vb[2].x+vb[5].x)/2.0f, 0, (vb[2].z+vb[5].z)/2.0f };
    center = (Vector3){0, 5, 0};

    fast_obj_destroy(mesh);

    InitWindow(width,height,"Game");
    SetTargetFPS(60);                       

    while(!WindowShouldClose()){
	   angle += 0.01f;
	   for(int i = 0; i < PositionsCount; i++){
		  rvb[i] = Screen(Mapping(TransformV(RotationY(angle, vb[i], center), (Vector3){0.0f, 0.0f, dz})));
	   }

	   BeginDrawing();
	   ClearBackground(BACKGROUND);

	   // for(int i = 0; i < LineCount; i++){
		  LinkVertex(rvb,ib,LineCount);
	   // }

	   EndDrawing();
    }

    CloseWindow();
    return 0;
}
