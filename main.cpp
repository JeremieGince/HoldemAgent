// System includes
#include <stdio.h>
#include <assert.h>
#include <iostream>

// CUDA runtime
#include <cuda.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

// Project includes
#include <Card.h>
#include <player.h>
#include <TexasHoldemGame.h>

using namespace game;
using namespace std;
/*

__global__ void add(int* a, int* b, int* c, int n) {
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	if (index < n)
		c[index] = a[index] + b[index];
}

void random_ints(int* x, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		x[i] = rand() % 10;
	}
}


#define N (2048*2048)
#define THREADS_PER_BLOCK 512


void main_test_cuda() {
	int* a, * b, * c; // host copies of a, b, c
	int* d_a, * d_b, * d_c; // device copies of a, b, c
	int size = N * sizeof(int);

	// Alloc space for device copies of a, b, c
	cudaMalloc((void**)&d_a, size);
	cudaMalloc((void**)&d_b, size);
	cudaMalloc((void**)&d_c, size);

	// Alloc space for host copies of a, b, c and setup input values
	a = (int*)malloc(size); random_ints(a, N);
	b = (int*)malloc(size); random_ints(b, N);
	c = (int*)malloc(size);

	// Copy inputs to device
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	// Launch add() kernel on GPU
	//add<<<N/THREADS_PER_BLOCK, THREADS_PER_BLOCK>>> (d_a, d_b, d_c);

	// Copy result back to host
	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

	// Cleanup
	free(a); free(b); free(c);
	cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
}
*/

int main(void) {

	Player player_rn0 = Player("rn0", "random");
	Player player_rn1 = Player("rn1", "random");

	vector<Player> players{ player_rn0, player_rn1 };

	TexasHoldemGame game = TexasHoldemGame(players, 10);

	game.start();

	game.doHand();

	int f;
	std::cin >> f;

	return 0;
}