#include <stdio.h>

int main(void)
{
	char line = 1;
	short stack_i = 0;
	char currently_building_int = 0;
	int num = 0;
	// unsigned int num : 17;
	int c;
	int stack[10];
	
	while (1) {

		c = getchar();
		if (c >= '0' && c <= '9') {
			if (stack_i >= 10) {
				goto error;
			}
			currently_building_int = 1;
			num = num * 10 + (c - '0');
			continue;;
		}
		else if (currently_building_int == 1) {
				stack[stack_i] = num;
				stack_i++;
				num = 0;
				currently_building_int = 0;
			}
		if (c == ' ') {
			continue;;
		}
		if (c == '\n'){
			goto complete_line;
		}
		if (c == EOF){
			goto end_of_file;
		}
		//------------- operator ------------
		if (stack_i < 2) {
			goto error;
		}
		stack_i--;
		int a =  stack[stack_i];
		stack_i--;
		int b =  stack[stack_i];

		if (c == '+'){
			stack[stack_i] = a+b;
			stack_i++;
		}
		else if (c == '-'){
			stack[stack_i] = b-a;
			stack_i++;
		}
		else if (c == '*'){
			stack[stack_i] = a*b;
			stack_i++;
		}
		else if (c == '/'){
			if ( a == 0) {
				goto error;
			}
			stack[stack_i] = b/a;
			stack_i++;
		}
		else {
			goto error;
		}
		continue;
		complete_line:

		stack_i--;
		int row_sum =  stack[stack_i];
		if (stack_i != 0) {
			printf("line %d: error at \\n\n", line);
			goto next_line;
		}
		
		printf("line %d: %d\n", line, row_sum);
		
		next_line:
		num = 0;
		currently_building_int = 0;
		stack_i = 0;
		line++;
		continue;
		error:
		printf("line %d: error at %c\n", line, c);
		while (c != '\n' && c != EOF) {
			c = getchar();
		}
		goto next_line;
	};
	end_of_file:
    return 0;
}

// ideas
// switch
// switch (c) {
// 	case '+':
// 		stack[stack_i] = a + b;
// 		stack_i++;
// 		break;
// 	case '-':
// 		stack[stack_i] = b - a;
// 		stack_i++;
// 		break;
// 	case '*':
// 		stack[stack_i] = a * b;
// 		stack_i++;
// 		break;
// 	case '/':
// 		if (a == 0) {
// 			goto error;
// 		}
// 		stack[stack_i] = b / a;
// 		stack_i++;
// 		break;
// 	default:
// 		goto error;
// }
