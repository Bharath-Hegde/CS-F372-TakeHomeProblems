#include <stdio.h>
#include<pthread.h>
#include <stdlib.h>

void *subthread_routine(void* param);
void takeMatrixInput(int matrix[][100], int p, int q);
void printMatrix(int matrix[][100], int rows, int cols);
void *computeCij(void* param);

int m,k,n;
int A[100][100], B[100][100], C[100][100];
typedef struct { 
	int row;
	int col;
} sret;

int main(){
	// main thread
	printf("Enter m: "); scanf("%d",&m);
	printf("Enter k: "); scanf("%d",&k);
	printf("Enter n: "); scanf("%d",&n);

	printf("\nEnter matrix A:\n");
	takeMatrixInput(A, m, k);
	printf("Enter matrix B:\n");
	takeMatrixInput(B, k, n);

	printf("\nA is:\n");
	printMatrix(A, m, k);
	printf("B is:\n");
	printMatrix(B, k, n);

	pthread_t tid;
	if(pthread_create(&tid, NULL, subthread_routine, NULL)){
		perror("1");
		exit(EXIT_FAILURE);
	}
	pthread_join(tid, NULL);

	printf("C is:\n");
	printMatrix(C, m, n);
}

void *subthread_routine(void* param){
	// subthread
	pthread_t sub_tids[m][n];
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			sret* ptr = malloc(sizeof(sret));
			ptr->row = i;
			ptr->col = j;
			if(pthread_create(&sub_tids[i][j], NULL, computeCij, ptr)) {
				perror("2");
				exit(EXIT_FAILURE);
			}
		}
	}

	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			void* var;
			if(pthread_join(sub_tids[i][j],&var)){
				perror("3");
				exit(EXIT_FAILURE);
			}
			C[i][j] = (long)(int*)var;
		}
	}

	pthread_exit(0);
}

void *computeCij(void* param){
	sret* temp = (sret*) param;
	int i = temp->row;
	int j = temp->col;
	free(temp);

	// compute C[i][j]
	int Cij;
	for (int var = 0; var < k; var++){
		Cij += A[i][var] * B[var][j];
	}
	pthread_exit((void*)(long)Cij);
}

void takeMatrixInput(int matrix[][100], int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			scanf("%d",&matrix[i][j]);
		}
	}
}

void printMatrix(int matrix[][100], int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}
