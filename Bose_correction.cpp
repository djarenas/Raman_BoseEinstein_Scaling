#include <cmath>
#include <fstream>
#include <iostream> 
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//Raman Data will be handled using this structure
struct Data_Point {double wavenumber; double scat_intensity;};

//Declare functions
vector<Data_Point> readFromFile(string filename); 
double calcScaleFactor(double wavenumber, double t_kelvin);
void scaleByBE(std::vector<Data_Point> &dv, double t_kelvin);
void printDataVector(std::vector<Data_Point> dv);
void writeToFile (std::vector<Data_Point> &dv, string filename); 

int main(int argc, char** argv)
{
    //Check command line had: input filename, temperature, and output filename.
    bool validCommandLine = true;
    if (argc != 4) {
        cout << "Syntax: a.exe <Input_Filename> <Temperature> <Output_Filename>" << endl;
        return 1;
    }

    string input_filename = argv[1];
    double temperature = std::stod(argv[2]);
    if (temperature <= 0) {
        cout << "Terminating. Temperature must be above zero." << endl;
        return 1;
    }
    string output_filename = argv[3];
    
    //Read/Check the Raman spectrum from the file -> Vector of data points.
    std::vector<Data_Point> data_vector;
    data_vector = readFromFile (input_filename); 
    if (data_vector.empty()) {
        cout << "Terminating. Input data was invalid." << endl;
        return 1;
    }
 
    //Scale the Raman scattering intensity by a factor related to the Bose-Einstein distribution
    //in order to obtain the imaginary part of the Raman susceptibility.
    scaleByBE(data_vector, temperature);
    
    //Save scaled Raman spectrum to filename specified in command line.
    writeToFile (data_vector, output_filename);

    return 0;
}


//Purpose: Reads a space or tab delimited file and converts it to a vector of Data_Point.
//The file should have two columns: 1) Wavenumbers, 2) Raman intensity.
vector<Data_Point> readFromFile(string filename){
    std::vector<Data_Point> data_vector = {}; //Return variable

    //Open filestream, check if it opens
    std::ifstream ifile (filename); 
    if (ifile.is_open()) { 
        cout << "Opening Raman spectrum file: " << filename << "\n";
    }
    else {
        cout << "Unable to open Raman spectrum file: " << filename << "\n"; 
        return data_vector;
    }

    string line; //A string that holds the information for each line
    Data_Point dp;
    bool successful_read = true;
    int r = 0; //Count rows in case you need to output an error and specify the row
    
    //Get each line from file. Make sure there are only two valid numbers in each line.
    while (std::getline(ifile, line)) {
        std::stringstream iss(line);
        iss >> dp.wavenumber;  //Test what happens if it gets a word instead of a number
        iss >> dp.scat_intensity;
        
        if (iss.fail()) {
            successful_read = false; 
            cout << "Bad line read. Row number: " << r + 1 << endl; 
            break;
        }
        if (!iss.eof()) {
            successful_read = false; 
            cout << "Too many columns. Row number: " << r + 1 << endl; 
            break;
        }

        r++;
        data_vector.push_back(dp);
    }
    
    //If a read was not successful, the return vector will be empty.
    if (!successful_read) {
        data_vector.clear();
    }
    
    ifile.close();
    return data_vector;
}


//Purpose: Console output a vector of Data_Point
void printDataVector(std::vector<Data_Point> dv){
    Data_Point dp;
    int rows = dv.size();
    for (int i = 0; i < rows; i++){
        cout << dv[i].wavenumber << "\t" << dv[i].scat_intensity << "\n";
    }
    return;
}


//Purpose: Calculate the scaling factor, from the Bose Einstein distribution,
//necessary to convert from Raman intensity to the imaginary (off-phase) part of the Raman 
//susceptibility. Wavenumber is the frequency in inverse cm, and temperature is in kelvin. 
double calcScaleFactor(double wavenumber, double temperature){
    //Further information on Bose Einstein scaling can be found in:
    //http://dx.doi.org/10.1088/0953-8984/15/19/332
    //https://digitalcommons.unl.edu/cgi/viewcontent.cgi?article=1263&context=chemfacpub

    //Photon energy scaled by thermal energy: (h*c/lambda)/(kb*T)
    //lambda is the wavelength. Wavenumber = 1/lambda.
    //T is temperature in kelvin.
    //h is Planck constant, c is speed of light. h*c = 1.98644x10(-23) J*cm.
    //kb is the Boltzman constant. kb = 1.38065x10(-23) J/K.
    //(h*c/lambda)/(kb*T) = (h*c/kb)*(wavenumber)/T.
    const double HC_KB = 1.4387; // h*c/kb 
    double exponent = HC_KB * wavenumber / temperature;
    
    //Bose einstein distribution: n(w) = 1/( exp[(h*c/lambda)/(kb*T)] - 1)
    double be = 1.0 / (exp(exponent) - 1.0);
    
    //Scaling of the raman scattering cross section to get the imaginary part of the Raman susceptibility
    double scaling = 1.0 / (be + 1.0);
    
    return scaling;
}


//Purpose: Scale the raman intensity in each data point by a factor calculated
//from the Bose Einstein distribution. Must input a vector of Data_Point and 
//temperature as kelvin. The original vector is modified in place. 
void scaleByBE(std::vector<Data_Point> &dv, double t_kelvin){
    for (vector<Data_Point>::iterator it = dv.begin(); it != dv.end(); ++it) {
        (*it).scat_intensity *= calcScaleFactor((*it).wavenumber, t_kelvin);
    }
}


//Write a vector of Data_Point to desired filename.
void writeToFile (vector<Data_Point> &dv, string filename){

    std::ofstream ofile (filename); //Open new file or rewrite over previous one.
    
    //Can you open the file?
    if (ofile.is_open()) {
        cout << "Saving results into : " << filename << "\n";
    }
    else {
        cout << "Could not open : " << filename << ". No data has been written." << endl;
    }

    for (vector<Data_Point>::iterator it = dv.begin(); it != dv.end(); ++it) {
        ofile << (*it).wavenumber << "\t" << (*it).scat_intensity << "\n";
    }
    
    //Close the file
    ofile.close();

    return;
}
