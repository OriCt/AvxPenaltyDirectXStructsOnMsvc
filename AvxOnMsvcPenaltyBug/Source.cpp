/** 
MIT License

Copyright (c) 2025 OriCt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */



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


