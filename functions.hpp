#include<iostream>
#include<filesystem>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<numeric>
#include<algorithm>

using namespace std;
namespace fs = std::__fs::filesystem;

float calc_area(vector<float> wavenumbes, vector<float> intensitys);
vector<float> calc_time(float increment, int number_exp);
vector<float> ln_maker(vector<float> no_ln);
vector<float> least_squares_fit(vector<float> x, vector<float> y);
float average(std::vector<float> const& v);
float Square(float val);
