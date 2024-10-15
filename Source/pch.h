#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES

#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>

using std::vector;
using std::cout;

#define DLLEXPORT extern "C" __declspec(dllexport)
#define Matrix4(i, j, k, m) vector<vector<vector<vector<double>>>>(i, vector<vector<vector<double>>>(j, vector<vector<double>>(k, vector<double>(m, 0))));
#define Matrix3(i, j, k) vector<vector<vector<double>>>(i, vector<vector<double>>(j, vector<double>(k, 0)));
#define Matrix(i, j) vector<vector<double>>(i, vector<double>(j, 0));

typedef vector<vector<vector<vector<double>>>> Matrix4;
typedef vector<vector<vector<double>>> Matrix3;
typedef vector<vector<double>> Matrix;
