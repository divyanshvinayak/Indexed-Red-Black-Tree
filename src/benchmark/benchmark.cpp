/*
* Copyright (c) 2020 Divyansh Vinayak
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "../indexed-red-black-tree/indexed_red_black_tree.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <algorithm>

using namespace std;
using namespace __rb_tree;

const int MAXN = 1e5;

void bench_naive(vector<int> &input_numbers, vector<int> &insert_positions, vector<int> &delete_positions)
{
	vector<int> vec;
	for (int i = 0; i < MAXN; i++)
	{
		vec.insert(vec.begin() + insert_positions[i], input_numbers[i]);
	}
	for (auto &pos : delete_positions)
	{
		vec.erase(vec.begin() + pos);
	}
}

void bench_indexed_red_black_tree(vector<int> &input_numbers, vector<int> &insert_positions, vector<int> &delete_positions)
{
	indexed_red_black_tree<int> tree;
	for (int i = 0; i < MAXN; i++)
	{
		tree.insert(insert_positions[i] + 1, input_numbers[i]);
	}
	for (auto &pos : delete_positions)
	{
		tree.erase(pos + 1);
	}
}

int main(int argc, char **argv)
{
	cout << "*** Indexed-Red-Black-Tree Benchmark ***" << endl;

	cout << "Generating input numbers ...";
	vector<int> input_numbers;
	while (input_numbers.size() < MAXN)
	{
		input_numbers.push_back(rand());
	}
	cout << " done" << endl;

	cout << "Generating insert positions ...";
	vector<int> insert_positions;
	while (insert_positions.size() < MAXN)
	{
		insert_positions.push_back(rand() % (insert_positions.size() + 1));
	}
	cout << " done" << endl;

	cout << "Generating delete positions ...";
	vector<int> delete_positions;
	while (delete_positions.size() < MAXN)
	{
		delete_positions.push_back(rand() % (delete_positions.size() + 1));
	}
	reverse(delete_positions.begin(), delete_positions.end());
	cout << " done" << endl;

	vector<tuple<chrono::high_resolution_clock::duration, chrono::high_resolution_clock::duration>> timings;

	cout << "Running ...";
	for (int i = 0; i < 10; i++)
	{
		auto start_time = chrono::high_resolution_clock::now();
		bench_naive(input_numbers, insert_positions, delete_positions);
		auto end_time = chrono::high_resolution_clock::now();
		auto time_1 = end_time - start_time;

		start_time = chrono::high_resolution_clock::now();
		bench_indexed_red_black_tree(input_numbers, insert_positions, delete_positions);
		end_time = chrono::high_resolution_clock::now();
		auto time_2 = end_time - start_time;

		timings.emplace_back(time_1, time_2);
	}
	cout << " done" << endl;

	cout << "Results: " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "  loop #" << i + 1;
		cout << ", naive: " << chrono::duration_cast<chrono::milliseconds>(get<0>(timings[i])).count() << "ms";
		cout << ", indexed red black tree: " << chrono::duration_cast<chrono::milliseconds>(get<1>(timings[i])).count() << "ms";
		cout << endl;
	}

	return 0;
}