#include <iostream>
#include "DirectXMath.h"
#include <vector>
#include <chrono>
#include <random>

class myVector : public DirectX::XMFLOAT3
{
public:
	myVector() : XMFLOAT3(0.f, 0.f, 0.f) {}
	myVector(float a, float b, float c) :XMFLOAT3(a, b, c) {};
	myVector(DirectX::FXMVECTOR V) { DirectX::XMStoreFloat3(this, V); }
	operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }
	myVector& operator+= (const myVector& V) { *this = DirectX::XMVectorAdd(*this, V); return *this; }
};

void Test(float a, float b, float c)
{
	myVector addVal(a, b, c);
	int iterations = 100;
	double total = 0.0;
	for (int i = 0; i < iterations; i++) {

		std::vector<myVector> arrVec(150000);
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < arrVec.size() - 1; i++) {
			arrVec[i] += addVal;
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		total += elapsed.count();
	}
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "Average Elapsed time: " << total / iterations << " ms\n";
	std::cout << "---------------------------------------------" << std::endl;
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	float a = dist(gen);
	float b = dist(gen);
	float c = dist(gen);
	Test(a, b, c);
	std::cout << "End" << std::endl;
}


