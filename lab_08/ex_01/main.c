extern int Check_square(int, int, int);
extern float My_division(float* a, float* b);

extern int _Matrix_Coordinates;
extern char _ROWS;
extern char _COLUMNS;

int main(void){
	int *matrix_pointer = &_Matrix_Coordinates;
	int r = _ROWS;
	int c = _COLUMNS;
	int radius = 5;
	int i, j;
	int x, y;
	int area = 0;
	volatile float pi;
		
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j = j + 2) {
			x = *matrix_pointer;
			y = *(matrix_pointer + 1);
			
			if(Check_square(x, y, radius))
				area++;
			
			matrix_pointer += 2;
		}
	}
	
	pi = My_division((float*) area, (float*) radius);
	
	//Exercise 2.1
	__asm__("svc 0xca");	//signature computed in svc is in R0
	
	//Exercise 2.2
	__asm__("svc 0xfe");
}
