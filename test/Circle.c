#include <raylib.h>
#include <math.h>

const int Width = 600;
const int Height = 600;

int main(){
    int cx, cy;
    cx = Width / 2;
    cy = Height / 2;
    int r = 20;

    InitWindow(Width, Height, "Circle");  // Initialize window and OpenGL context
    while(!WindowShouldClose()){
	   BeginDrawing();
	   ClearBackground(BLACK);
	   for (int i = 0; i < Width; i++){
		  for (int j = 0; j < Height; j++){
			 if(pow(i - cx, 2) + pow(j - cy, 2) < r * r)
				DrawPixel(i, j, WHITE);
		  }
	   }

	   DrawLine(10, 10, 10, 100, RED);
	   DrawLine(10, 10, 100, 10, BLUE);

	   EndDrawing();
    }
}
