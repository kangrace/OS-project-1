// Grace Kang
// CSCI 3453
// Lab Assignment 1 due Monday Sept. 25

#include <pthread.h>;		// for windows it is <windows.h>
#include <stdio.h>;			// file I/O

// variables
int matrixA[100][100];	// first matrix
int aCol;		// holds column from matrixA 

int matrixB[100][100];	// second matrix
int bRow;		// holds row from matrixB

int matrixR[100][100];	// result matrix
int resultRow;	// holds resultMatrix row from matrixA
int resultCol;	// holds resultMatrix column from matrix B


// struct for each thread
struct threadData
{
	int tRow;	// row
	int tCol;	// col
};

// function prototypes
void matrixMultiplication(int, int);
void *worker(void*);
void allocateMatrix(int*, int, int);
void getMatrix();
void printMatrix(int, int, int);




int main(int argc, const char *argv[])
{
	getMatrix();	// get matrices from input file
	
	int threads = atoi(argv[1]);	// number of threads to use
	int i, j;

	// assign thread to row and column
	struct threadData *data = (struct v *) malloc(sizeof(struct threadData));
	data->tRow = i;
	data->tCol = j;

	// initialize thread
	pthread_t *thread;
	pthread_attr_t attr;		// thread attributes
	pthread_attr_init(&attr);	// get defualt attributes

	// create new thread
	pthread_create(&thread, &attr, worker, data);

	// join all threads so parents wait for all threads to finish
	// block until all threads complete
	for (i = 0; i < threads; ++i)
	{
		pthread_join(thread[i], NULL);

	}
	
	// print input matrices A and B
	printf("Matrix A: \n");
	printMatrix(matrixA, resultRow, aCol);
	printf("Matrix B: \n");
	printMatrix(matrixB, bRow, resultCol);

	//
	// need to output "created worker thread ### for row #, etc"
	//

	// print result matrix R
	printf("Matrix C: \n");
	printMatrix(matrixR, resultRow, resultCol);


	// ending now?
	// free allocated space for each thread
	if (thread != NULL)
	{
		free(thread);
		thread = NULL;
	}
	return 0;
}

//********************************************
// function to get the matrix from input file
//********************************************
void getMatrix()
{
	// get matrix from input file called matrix.txt
	FILE *infile;
	infile = fopen("matrix.txt", "r");

	if (infile == NULL)
	{
		printf("Error Reading File\n");
		exit(0);
	}

	int dimRow;			// holds first number from file & for matrix B
	int dimCol;			// holds second number from file & for matrix B

	// input to matrix A
	fscanf(infile, "%d %d", &dimRow, &dimCol);	// get first and second numbers for matrix A
	resultRow = dimRow;							// assign row for result matrix
	aCol = dimCol;								// save column number

	allocateMatrix(&matrixA, dimRow, dimCol);
	
	fscanf(infile, "%*[^\n]\n", NULL);			// move to the next line
	for (int i = 0; i < dimRow; i++)
	{
		for (int j = 0; j < dimCol; j++)
		{
			if (fscanf(infile, "%d", matrixA[i][j]) != 1) // if it is not a number
			{
				printf("Unexpected element in input file.");
				return(1);	// return non-zero number
			}
		}
	}

	// input to matrix B
	fscanf(infile, "%d %d", &dimRow, &dimCol);	// get first and second numbers for matrix B
	resultCol = dimCol;							// assign col for result matrix
	bRow = dimRow;								// save row number

	allocateMatrix(&matrixB, dimRow, dimCol);
	
	fscanf(infile, "%*[^\n]\n", NULL);			// move to the next line
	for (int i = 0; i < dimRow; i++)
	{
		for (int j = 0; j < dimCol; j++)
		{
			if (fscanf(infile, "%d", &matrixB[i][j]) != 1) // if it is not a number
			{
				printf("Unexpected element in input file.");
				return(1);	// return non-zero number
			}
		}
	}

	// make room for the result matrix
	allocateMatrix(&matrixR, resultRow, resultCol);

	fclose(infile);		// close file
	return 0;
}

//********************************************
// function to allocate memory for a matrix
//********************************************
void allocateMatrix(int *matrix, int row, int col)
{
	*matrix = (int)malloc(row * sizeof(int));
	for (int i = 0; i < row; i++)
		(matrix)[i] = malloc(col * sizeof(int));
}

//********************************************
// function matrix multiplication
//********************************************
void matrixMultiplication(int row, int col)
{
	int sum = 0;
	for (int i = 0; i < row; i++)
	{
		sum = sum + matrixA[row][i] * matrixB[i][col];
	}
	matrixR[row][col] = sum;
}

//********************************************
// function worker for pthread
//********************************************
void *worker(void *param)
{
	struct threadData *data = param;
	matrixMultiplication(data->tRow, data->tCol);

	// exit thread
	pthread_exit(0);
}

//********************************************
// function to print the matrix
//********************************************
void printMatrix(int matrix[100][100], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// output the matrix
			printf("%d", matrix[i][j]);
		}
	}
	// output newline
	printf("\n");
}
