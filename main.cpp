/*
-*-     Spectra Analyzer v2     -*-
-*-     Created by Marcel Ruth  -*-
Doctoral student and Kekulé-Fellow

Iterates through a given folder and integrates bands that 
you chose.
*/

#include "functions.hpp"

int main(int argc, char* argv[])
{
    string input_path = argv[1];
    float higher_wavenumber;
    float lower_wavenumber;
    int high_wave_index;
    int low_wave_index;
    float time_inc;

    if (argc > 3)
    {
        string str_higher_wavenumber = argv[2];
        string str_lower_wavenumber = argv[3];
        string str_time_inc = argv[4];

        higher_wavenumber = stof(str_higher_wavenumber);
        lower_wavenumber = stof(str_lower_wavenumber);
        time_inc = stof(str_time_inc);
    } 

    string line;
    string delimiter = (",");

    float wavenumber;
    float intensity;

    struct Measurement
    {
        vector<float> wavenumbers;
        vector<float> intensitys;
    };

    vector<Measurement> experiments;

    vector<float> mea_wnums = {};
    vector<float> mea_inten = {};
    
    for (const auto &entry : fs::recursive_directory_iterator(input_path))
	{
        // clear the measuremnt struct
        Measurement measurement = {};
        cout << "Read in: " << entry.path() << endl;

        ifstream CurrentFile(entry.path());
        if (CurrentFile.is_open())
        {
            while (getline(CurrentFile, line))
            {   
                wavenumber = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length()); 
                intensity = stof(line);

                measurement.wavenumbers.push_back(wavenumber);
                measurement.intensitys.push_back(intensity);
            }
        }
        // at the end of reading append the measurement to the experiment structure
        experiments.push_back(measurement);
        }
    // The data is now loaded into experiments and the fun can begin
    std::cout << "Data succesfully loaded!" << endl;
    // Iterate through the experiments and compute the area under the curve for
    // each measurement
    vector<float> all_areas;
    vector<float> exp_time = calc_time(time_inc, experiments.size());
    if (argc > 3)
    {
        for (auto it = experiments.begin(); it != experiments.end(); ++it)
        {
            vector<float> mea_wnums = {};
            vector<float> mea_inten = {};

            cout << it->wavenumbers[0] << "   " << it->intensitys[0] << endl;
            // find the two bands (index) in wavenumbers
	    cout << "back " << it->wavenumbers.back() << endl; 	    
	    cout << "begin " << *it->wavenumbers.begin() << endl;
	    cout << "end " << *it->wavenumbers.end() << endl;
	    cout << higher_wavenumber << " " << lower_wavenumber << endl;
	    vector<float>::iterator high_wave = find(it->wavenumbers.begin(), it->wavenumbers.end(), higher_wavenumber);
	    high_wave_index = distance(it->wavenumbers.begin(), high_wave);
            vector<float>::iterator low_wave = upper_bound(it->wavenumbers.begin(), it->wavenumbers.end(), lower_wavenumber);
            low_wave_index = distance(it->wavenumbers.begin(), low_wave);
	    cout << low_wave_index << " " << high_wave_index << endl;
            for (int index_it = high_wave_index; index_it <= low_wave_index; index_it++)
	    {
                mea_wnums.push_back(it->wavenumbers[index_it]);
                mea_inten.push_back(it->intensitys[index_it]);
            }
	    // compute the area
            vector<float> line_inten = {mea_inten.front(), mea_inten.back()};
            vector<float> line_wnums = {mea_wnums.front(), mea_wnums.back()};
            
	    float area_curve = calc_area(mea_wnums, mea_inten); // calculate are of peak
            
	    float area_line = calc_area(line_wnums, line_inten); // calculat straight line area
            float area_peak = area_curve - area_line; // calculate resulting area
            all_areas.push_back(area_peak);
        } 
        // areas are now calculated, save areas in file
        ofstream ExpData("ExpData.csv");
        ExpData << "Time_h" << "Area_au" << endl;
        for (int i = 0; i < all_areas.size(); i++)
        {
            ExpData << exp_time[i] << "," << all_areas[i] << endl;
        }
        // fit to y = a + b ln(x), just like y = mx + b
        vector<float> ln_exp_time = ln_maker(exp_time);
        vector<float> coeff = least_squares_fit(ln_exp_time, all_areas); // bias first, slope second
        cout << "Fit for logarithmic function to AREA = Bias + Slope*ln(TIME) is:" << endl;
        cout << "Bias: " << coeff.front() << " --- Slope: " << coeff.back() << endl;
        float halftime = log(2)/coeff.back();
        cout << "t_0.5 " << halftime << endl;
    }
};
