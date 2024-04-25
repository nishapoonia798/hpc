#include <iostream>
#include <omp.h>

using namespace std;

int minval(int arr[], int n){
	int minval = arr[0];
	#pragma omp parallel for reduction(min : minval)
		for (int i = 0; i < n; i++) {
			if (arr[i] < minval) minval = arr[i];
		}
	return minval;
}

int maxval(int arr[], int n){
	int maxval = arr[0];
	#pragma omp parallel for reduction(max : maxval)
		for (int i = 0; i < n; i++) {
			if (arr[i] > maxval) maxval = arr[i];
		}
	return maxval;
}

int sum(int arr[], int n){
	int sum = 0;
	#pragma omp parallel for reduction(+ : sum)
		for (int i = 0; i < n; i++) {
			sum += arr[i];
		}
	return sum;
}

double average(int arr[], int n) {
	return (double)sum(arr, n) / n;
}

int main(){
	int n = 10;
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	cout << "\nData: ";
	for (int i : arr) {
		cout << i << " ";
	}
	cout << "\n\n";

	cout << "min: " << minval(arr, n) << '\n';
	cout << "max: " << maxval(arr, n) << '\n';
	cout << "sum: " << sum(arr, n) << '\n';
	cout << "avg: " << average(arr, n) << '\n';
}