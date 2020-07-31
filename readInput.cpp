#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void get_line_numbers(string, int *);

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
    string str_starting_vals    = "Starting Values";
    // String values containing the line number of the certified values
    string str_certified_vals   = "Certified Values";
    // String values containing the line number of the data
    string str_data_vals        = "Data ";

    //Boolean for each of the above string identifiers (true if found)
    bool found_starting_vals   = false;
    bool found_certified_vals  = false;
    bool found_data_vals       = false;
    
    //  size_t variable keeping track of strings to find
    size_t found_str;

    int idx_starting_vals[]   = {-1, -1};
    int idx_certified_vals[]  = {-1, -1};
    int idx_data_vals []  = {-1, -1};
    // Integer keeping track of line numbers. First line is 1 (not zero)
    int line_number = 0;
    while(getline(data_file, line)){
        line_number += 1;
        // Get starting values
        if(!found_starting_vals){
            found_str = line.find(str_starting_vals);
            if(found_str != string::npos){
                // Found string
                str_starting_vals = line;
                get_line_numbers(line, idx_starting_vals);
                cout << "Starting values: " << idx_starting_vals[0] << "\t" << idx_starting_vals[1] << endl;
                found_starting_vals = true;
                continue;
            }
        }
        // Get certified values
        if(!found_certified_vals){
            found_str = line.find(str_certified_vals);
            if(found_str != string::npos){
                // Found string
                str_certified_vals = line;
                get_line_numbers(line, idx_certified_vals);
                cout << "Certified values: " << idx_certified_vals[0] << "\t" << idx_certified_vals[1] << endl;
                found_certified_vals = true;
                continue;
            }
        }
        // Get data lines
        if(!found_data_vals){
            found_str = line.find(str_data_vals);
            if(found_str != string::npos){
                // Found string
                str_data_vals = line;
                get_line_numbers(line, idx_data_vals);
                cout << "Data values: " << idx_data_vals[0] << "\t" << idx_data_vals[1] << endl;
                found_data_vals = true;
                continue;
            }
        }

    data_file.close();
}



void get_line_numbers(string str, int * idx){
    // Input: string containing lines information. E.g., "lines 41 to 43". 
    // @params@input: num_ints: Number of expected values
    // Output: 2-element array

    stringstream ss;
    ss << str; 
    // Initialize values to -1 both
    // Variable used to indicate whether an iteger is found or not
    int found;
    // Temporary storage
    string temp;
    while(!ss.eof()){
        ss >> temp;
        if(stringstream(temp)>>found){
            if(idx[0] == -1){
                idx[0] = found;
            }else{
                idx[1] = found;
                break;
            }
        }
    }
}