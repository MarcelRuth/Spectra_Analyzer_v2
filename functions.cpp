#include "functions.hpp"

float calc_area(vector<float> wavenumbers, vector<float> intensitys)
{
    // https://en.wikipedia.org/wiki/Trapezoidal_rule
    // Uniform grid is given in the spectral data

    // front and back are exchanged in contrast to wiki, cause the wavenumbers are switched as well

    float delta_x = (wavenumbers.front() - wavenumbers.back()) / wavenumbers.size(); // Δx = (b - a) / N
    float area_integral;
    float running_sum;

    for (size_t i=intensitys.size() - 1; i != 1; i--) // iterate reversely, because wavenumbers are higher first...
    {
        running_sum = running_sum + (intensitys[i] + ((intensitys.front() + intensitys.back()) / 2));
    }
    area_integral = delta_x + running_sum;

    return area_integral;
};

vector<float> calc_time(float increment, int number_exp)
{
    vector<float> timevec;
    float cur_time;
    for (int i = 0; i < number_exp; i++)
    {
        cur_time = cur_time + increment;
        timevec.push_back(cur_time);
    }
    return timevec;
};

vector<float> ln_maker(vector<float> no_ln)
{
    vector<float> ln_vector;
    for (int i = 0; i < no_ln.size(); i++)
    {
        ln_vector.push_back(log(no_ln[i]));
    }
    return ln_vector;
};

float average(vector<float> const& vec){
    if(vec.empty())
    {
        return 0;
    }

    float sum;
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        sum = sum + *it;
    }
    float average = sum / vec.size();

    return average;
};

float Square(float val)
{
    return (val*val);
};

vector<float> least_squares_fit(vector<float> x, vector<float> y)
{    
    // https://en.wikipedia.org/wiki/Least_squares

    float mean_x = average(x);
    float mean_y = average(y);

    float numenator;
    float denomenator;

    for (size_t i = 0; i < x.size(); i++)
    {
        numenator = numenator + (x[i] - mean_x) * (y[i] - mean_y);
        denomenator = denomenator + Square((x[i] - mean_x));
    } 

    float slope = numenator / denomenator;
    float bias = mean_y - (slope * mean_x);

    vector<float> coeff;
    coeff.push_back(bias);
    coeff.push_back(slope);

    return coeff;
};