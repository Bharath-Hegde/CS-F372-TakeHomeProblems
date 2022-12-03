#include <stdio.h>
#include<pthread.h>
#include <stdlib.h>

typedef struct {
	int d1;
	int d2;
} dig_struct;

int findNumberLength(int n){
	int count = 0;
	if (n) {
		while(n > 0){
			count++;
			n /= 10;
		}
		return count;
	}

	return 1;
}

void findDigits(int digits[], int n){
	int count = 0;
	if (n) {
		while(n > 0){
			digits[count] = n%10;
			n /= 10;
			count++;
		}
	}
	else digits[0] = 0;
}

void printArray(int arr[], int n){
	for(int i = 0; i < n; i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

void printMatrix(int matrix[][100], int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}

void *subroutine(void* param){
	dig_struct* digs_ptr = (dig_struct*) param;
	int d1 = digs_ptr->d1, d2 = digs_ptr->d2;
	free(digs_ptr);
	pthread_exit((void*)(long)(d1*d2));
}

int main(){
	int n1, n2;
	printf("Enter first number: "); scanf("%d",&n1);
	printf("Enter second number: "); scanf("%d",&n2);

	int l1 = findNumberLength(n1), l2 = findNumberLength(n2);
	int digits1[l1], digits2[l2];

	findDigits(digits1, n1);
	findDigits(digits2, n2);

	int products[l1+l2];
	for (int i = 0; i < l1+l2; i++) products[i] = 0;
	pthread_t tids[l2][l1];
	for (int i = 0; i < l2; i++){
		for (int j = 0; j < l1; j++){
			dig_struct* digs_ptr = malloc(sizeof(dig_struct));
			digs_ptr->d1 = digits1[j];
			digs_ptr->d2 = digits2[i];
			
			if(pthread_create(&tids[i][j], NULL, subroutine, digs_ptr)){
				perror("1");
				exit(EXIT_FAILURE);
			}
		}
	}

	for (int i = 0; i < l2; i++){
		for (int j = 0; j < l1; j++){
			void* var;
			if(pthread_join(tids[i][j], &var)){
				perror("2");
				exit(EXIT_FAILURE);
			}
			products[i+j] += (long)(int*) var;
		}
	}

	// ans
	int final = 0;
	for (int i = 0; i < l1+l2; i++) {
		int temp = 1;
		for (int j = 0; j < i; j++) temp *= 10;
		if (products[i]/10) { // double digit
			final += temp*(products[i]%10);
			products[i+1] += products[i]/10; // carry forward
		} else final += temp*products[i]; // single digit
	}
	printf("%d x %d = %d\n", n1, n2, final);
}
