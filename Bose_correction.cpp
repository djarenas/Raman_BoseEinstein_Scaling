#include <iostream> 
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <typeinfo>
/*
    Convention
    Class_Names
    functionName
    variable_name
    CONSTANT_NAMES
*/
using std::cout;
using std::cin;
using std::endl;
using std::string;

//Raman Data will be handled using this structure
struct datapoint {double wavenumber; double scat_intensity;};

//Declare functions
int playMenu();
int checkFileNColumns (string filename, int n);
std::vector<datapoint> readFileintoDataPoints (string filename);
double calcScalingBE(double wavenumber, double t_kelvin);
std::vector<datapoint> scalebyBE(std::vector<datapoint> dv, double t_kelvin);
void printDataVector(std::vector<datapoint> dv);
void saveDVectorintoFile (std::vector<datapoint> dv, string filename);


int main(int argc, char** argv)
{

    //Open the file with the name input by the user in command line. Alert user if unable to open.
    string file_name = argv[1];
    std::ifstream ifile (file_name);
    if (ifile.is_open()){ cout << "Opening Raman spectrum file: " << file_name << "\n";}
    else {
        cout << "Unable to open Raman spectrum file: " << file_name << "\n Terminating."; 
        return 0;
    }
    
    //Read the Raman spectrum from the file into a vector of data points
    std::vector<datapoint> data_vector;   
    if(checkFileNColumns(file_name, 2) != 0){cout << "Improper input file. Terminating.\n"; return 0;}
    data_vector = readFileintoDataPoints ("Bi25FeO39-Powder.txt");
 
    //Scale the Raman scattering intensity by a factor related to the Bose-Einstein distribution
    //in order to obtain the imaginary part of the Raman susceptibility 
    data_vector = scalebyBE(data_vector, std::stod(argv[2]));
    
    //Save scaled Raman spectrum to filename specified in command line
    saveDVectorintoFile (data_vector, argv[3]);


    return 0;
}

int checkFileNColumns (string filename, int n){
    //Purpose: Reads the spectrum file and checks it has two valid columns

    //Declare variables
    std::ifstream ifile (filename); //File object
    string line; //A string that holds the information for each line

    int m = 0; //Count the number of rows
    int e = 0; //Count the number of rows with improper columns
    while (std::getline(ifile, line)) //Get each line from file. Then, while you have lines...
    {
        m++;
        std::stringstream iss(line);
        string sinput;
        int s = 0; //Count columns
        while(iss >> sinput){
            s++; //Count each column
        }
        if (s != 2){
            cout << "In row " << m << " the number of columns were not equal to the desired " << n << " columns.\n";
            e++;
        }
    }
    ifile.close();
    cout << "Number of rows: " << m << "\n";

    return e; 
}

std::vector<datapoint> readFileintoDataPoints (string filename){
    //Purpose: Reads a file and converts it to a vector of datapoints

    //Declare variables
    std::ifstream ifile (filename); //File object
    string line; //A string that holds the information for each line
    datapoint new_dp;
    std::vector<datapoint> data_vector;

    while (std::getline(ifile, line)) //Get each line from file. Then, while you have lines...
    {
        std::stringstream iss(line);
        iss >> new_dp.wavenumber;
        iss >> new_dp.scat_intensity;
        data_vector.push_back(new_dp);
    }
    return data_vector;
}

void printDataVector(std::vector<datapoint> dv){
    datapoint dp;
    int rows = dv.size();
    for (int i = 0; i < rows; i++){
        cout << dv[i].wavenumber << "\t" << dv[i].scat_intensity << "\n";
    }
    return;
}

double calcScalingBE(double wavenumber, double t_kelvin)
{
    /*
    wavenumber must have units of inverse cm
    temperature must be in kelvin
    http://dx.doi.org/10.1088/0953-8984/15/19/332
    https://digitalcommons.unl.edu/cgi/viewcontent.cgi?article=1263&context=chemfacpub
    */
    //const int: Read-only variable
    const double HC_KB = 1.4387;  //hc/kb = 1.98644x10(-23) J*cm / 1.38065x10(-23) J/K

    // Photon energy scaled by thermal energy: 
    // hw/kbT =(h*c/lambda)/(kb*T) = (h*c/kb)*(1/lambda)/T
    double pe_te = HC_KB*wavenumber/t_kelvin;
    
    //Bose einstein distribution: n(w) = 1/( e(hw/kbT) - 1)
    double be = 1.0/(exp(pe_te) -1.0);
    
    //Scaling of the raman scattering cross section to get the imaginary part of the Raman susceptibility
    double scaling = 1.0/(be+1.0);
    
    return(scaling);
}

std::vector<datapoint> scalebyBE(std::vector<datapoint> dv, double t_kelvin){
    for (int i = 0; i < dv.size(); i++){
        dv[i].scat_intensity = calcScalingBE(dv[i].wavenumber, t_kelvin)*dv[i].scat_intensity;
    }
    return(dv);
}

void saveDVectorintoFile (std::vector<datapoint> dv, string filename){
    //Save a vector of objects with the datapoint structure into a file

    std::ofstream ofile (filename); //Open new file or rewrite over previous one.
    if(ofile.is_open()){cout << "Saving results into : " << filename << "\n";}
    else{cout << "Could not open : " << filename << "\n";}
    datapoint dp;
    int rows = dv.size();
    for (int i = 0; i < rows; i++){
        ofile << dv[i].wavenumber << "\t" << dv[i].scat_intensity << "\n";
    }
    return;
}

int playMenu(){
    int choice;
    cout << "Raman shift units?" << std::endl;
    cout << "0. Wavenumbers (cm-1) \n1. eV" << std::endl;
    cin >> choice;
    //Confirm the choice to the user
    switch(choice){
        case 0:
            cout << "You chose wavenumbers a.k.a. cm-1\n"; 
            break;
        case 1:
            cout << "You chose electron Volts (ev)\n"; 
            break;
        default:
            cout << "Invalid input\n";
            break;
    }

    return (choice);   
}