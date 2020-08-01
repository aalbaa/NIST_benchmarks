#include <iostream>
#include <map>
#include <string>

#include "NIST_Parser.cpp"

using namespace std;

int main(){
    string file_name = "Chwirut2_dat.txt";
    NIST_Parser nist_parser(file_name);

    double** data_vals = nist_parser.data_vals();
    int num_observations = nist_parser.num_observations();

    // Print the parameter values
    for(int i = 0; i < num_observations; i++){
        for(int j = 0; j < 2; j++){
            cout << data_vals[i][0] << "\t" << 
                data_vals[i][1] << endl;
        }
    }

}