#include <iostream>
#include <chrono>
using namespace std;

__global__ void multiply(int* A, int* B, int* C, int size) {
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < size && col < size) {
		int sum = 0;
		for (int i = 0; i < size; i++) {
			sum += A[row * size + i] * B[i * size + col];
		}
		C[row * size + col] = sum;
	}
}

void initialize(int* matrix, int size) {
	for (int i = 0; i < size * size; i++) {
		matrix[i] = rand() % 10;
	}
}

void print(int* matrix, int size) {
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			cout << matrix[row * size + col] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

int main() {
	int* A, * B, * C;

	int N;
	cout << "Enter matrix size (min. 2): ";
	cin >> N;
	cout << endl;

	int matrixSize = N * N;
	size_t matrixBytes = matrixSize * sizeof(int);

	A = new int[matrixSize];
	B = new int[matrixSize];
	C = new int[matrixSize];

	initialize(A, N);
	initialize(B, N);

	cout << "Matrix A: \n";
	print(A, N);

	cout << "Matrix B: \n";
	print(B, N);

	auto t_start = chrono::high_resolution_clock::now();
		
		int* X, * Y, * Z;
		cudaMalloc(&X, matrixBytes);
		cudaMalloc(&Y, matrixBytes);
		cudaMalloc(&Z, matrixBytes);

		cudaMemcpy(X, A, matrixBytes, cudaMemcpyHostToDevice);
		cudaMemcpy(Y, B, matrixBytes, cudaMemcpyHostToDevice);

		int THREADS = 2;

		int BLOCKS = N / THREADS;

		dim3 threads(THREADS, THREADS);
		dim3 blocks(BLOCKS, BLOCKS);

		multiply<<<blocks, threads>>>(X, Y, Z, N);

		cudaMemcpy(C, Z, matrixBytes, cudaMemcpyDeviceToHost);


	auto t_end = chrono::high_resolution_clock::now();

	cout << "Multiplication of matrix A and B: \n";
	print(C, N);

	cout
		<< "Time taken: "
		<< chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count()
		<< " ms"
	;

	delete[] A;
	delete[] B;
	delete[] C;

	cudaFree(X);
	cudaFree(Y);
	cudaFree(Z);
}