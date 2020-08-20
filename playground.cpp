#include <iostream>
#include <map>
#include <string>
#include <Eigen/Dense>

#include "NIST_Parser.cpp"

using namespace std;
using namespace Eigen;

MatrixXd getOutputMatrix(double**, int, int);
MatrixXd getInputMatrix(double**, int, int, int);

int main(){
    string file_name = "Chwirut2_dat.txt";
    NIST_Parser nist_parser(file_name);

    double** data_vals = nist_parser.data_vals();

    // // Matrix of the output values
    // MatrixXd outputMatrix(nist_parser.num_observations(), nist_parser.num_output());
    // // Matrix of input values
    // MatrixXd inputMatrix(nist_parser.num_observations(), nist_parser.num_input());

    // nist_parser.
    // Convert the double array into a Matrix
    int num_observations = nist_parser.num_observations();

    // // Print the parameter values
    // for(int i = 0; i < num_observations; i++){
    //     // First, the outputs
    //     for(int j = 0; j < nist_parser.num_output(); j++){
    //         outputMatrix(i, j) = data_vals[i][j];
    //     }
    //     // Now, the inputs
    //     for(int j = 0; j < nist_parser.num_input(); j++){
    //         inputMatrix(i, j) = data_vals[i][nist_parser.num_output() + j];
    //     }
    // }
    MatrixXd outputMatrix = getOutputMatrix(data_vals, nist_parser.num_observations(), 
            nist_parser.num_output());
    MatrixXd inputMatrix = getInputMatrix(data_vals, nist_parser.num_observations(), 
            nist_parser.num_output(), nist_parser.num_input());

    MatrixXd A = MatrixXd::Random(2, 2);
    cout << A << endl;
}


MatrixXd getOutputMatrix(double** data_vals, int num_observations, int num_output){
    // Function to convert the double array into matrix. 
    // This function returns the output values

    // Matrix of the output values
    MatrixXd outputMatrix(num_observations, num_output);
    for(int i = 0; i < num_observations; i++){
        // First, the outputs
        for(int j = 0; j < num_output; j++){
            outputMatrix(i, j) = data_vals[i][j];
        }
    }
    return outputMatrix;
}

MatrixXd getInputMatrix(double** data_vals, int num_observations, int num_output, int num_input){
    // Function to convert the double array into matrix. 
    // This function returns the input values

    // Matrix of the output values
    MatrixXd inputMatrix(num_observations, num_input);
    for(int i = 0; i < num_observations; i++){
        // First, the outputs
        for(int j = 0; j < num_input; j++){
            inputMatrix(i, j) = data_vals[i][num_output + j];
        }
    }
    return inputMatrix;
}