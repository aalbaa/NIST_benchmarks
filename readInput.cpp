#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

void get_line_numbers(string, int *, int);
void get_line_numbers(string, double *, int);

int main(){
    string file_name = "Chwirut2_dat.txt";
    
    // Input file stream
    ifstream data_file(file_name);

    // Open file if not open
    if(!data_file.is_open()){
        data_file.open(file_name);
    }

    // Reading line-by-line
    string line; 

    // String that contains the line number of starting and ending values (e.g., "lines 41 to 43")
    string str_params_int[] = {"Starting Values", "Certified Values", "Data ", 
            " Response", " Predictor", " Observations", " Parameters", 
            "Degrees of Freedom", "Number of Observations"};
    // Total number of strings to be found
    int num_str_params_int = 9;

    // // String of parameters that take double as values.
    // string str_params_double[] = {"Residual Sum of Squares", "Residual Standard Deviation"};
    // int num_str_params_int = 2;
    
    //  size_t variable keeping track of strings to find
    size_t found_str;

    // Map of starting and ending indices ("start", "certified", "data")
    int idx_vals[num_str_params_int][2];
    // Initialize the values to -1 (aribtrarily)
    for(int i = 0; i<num_str_params_int; i++){
        for(int j = 0; j<2; j++){
            idx_vals[i][j] = -1;
        }
    }

    // Integer keeping track of line numbers. First line is 1 (not zero)
    int line_number = 0;

    // Index of strings found so far
    int idx_strings_found = 0;

    // Number of integers to be found varies between 1 and 2
    int num_ints;

    // Residual sum of squares
    string str_residual[] = {"Residual Sum of Squares", "Residual Standard Deviation"};
    // string str_residual_sum = "Residual Sum of Squares";
    // // Residual standard deviation
    // string str_residual_std = "Residual Standard Deviation";

    // Residual values
    double residual_arr[2];
    // double residual_sum;
    // // Residual standard deviation
    // double residual_std;
    int num_str_params_double;
    int idx_str_params_double = 0;
    while(getline(data_file, line) && idx_strings_found < num_str_params_int){
        line_number++;
        // Get starting values
        found_str = line.find(str_params_int[idx_strings_found]);
        if(found_str != string::npos){
            // Found string. Next string
            (idx_strings_found < 3) ? (num_ints = 2) : (num_ints = 1);
            
            get_line_numbers(line, idx_vals[idx_strings_found], num_ints);
            // get_line_numbers(line, idx_starting_vals, 2);
            cout << idx_vals[idx_strings_found][0] << "\t" << idx_vals[idx_strings_found][1] << endl;
            idx_strings_found++;
            continue;
        }
        // Otherwise, look for the residual values
        found_str = line.find(str_residual[idx_str_params_double]);
        if(found_str != string::npos){
            // Add value to the array address so to store parameters in the appropriate element locations
            get_line_numbers(line,  residual_arr+idx_str_params_double, 1);
            idx_str_params_double++;
            continue;
        }
    }
    
    cout << "Line number: " << line_number << endl;

    // // Close and re-open file
    // data_file.close();
    // data_file.open(file_name);

    // // /*******************************
    // // // Get the data
    // // Number of data_points
    // // *******************************/

    // cout << "Let's see what's left. Uhmmm" << endl;
    // while(getline(data_file, line)){
    //     line_number++;
    // }
    
    data_file.close();
}



void get_line_numbers(string str, double * vals, int num_vals){
    // Input: string containing lines information. E.g., "lines 41 to 43". 
    // @params@input: num_vals: Number of expected (double) values
    // Output: 2-element array

    stringstream ss;
    ss << str; 
    // Initialize values to -1 both
    // Variable used to indicate whether an iteger is found or not
    double found;
    // Temporary storage
    string temp;
    // The next index to be filled
    int idx_found = 0;
    while(!ss.eof() && idx_found < num_vals){
        ss >> temp;
        if(stringstream(temp)>>found){
            vals[idx_found] = found;
            idx_found++;
        }
    }
}
void get_line_numbers(string str, int * idx, int num_ints){
    // Input: string containing lines information. E.g., "lines 41 to 43". 
    // @params@input: num_ints: Number of expected (integers) values
    // Output: 2-element array

    stringstream ss;
    ss << str; 
    // Initialize values to -1 both
    // Variable used to indicate whether an iteger is found or not
    int found;
    // Temporary storage
    string temp;
    // The next index to be filled
    int idx_found = 0;
    while(!ss.eof() && idx_found < num_ints){
        ss >> temp;
        if(stringstream(temp)>>found){
            idx[idx_found] = found;
            idx_found++;
        }
    }
}