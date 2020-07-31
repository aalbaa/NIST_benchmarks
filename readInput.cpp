#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

void get_line_numbers(string, int *, int);

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
    string str_to_find[] = {"Starting Values", "Certified Values", "Data "};
    // Total number of strings to be found
    int num_str_to_find = 3;
    
    
    //  size_t variable keeping track of strings to find
    size_t found_str;

    // Map of starting and ending indices ("start", "certified", "data")
    int idx_vals[3][2];
    // Initialize the values to -1 (aribtrarily)
    for(int i = 0; i<3; i++){
        for(int j = 0; j<2; j++){
            idx_vals[i][j] = -1;
        }
    }

    // Integer keeping track of line numbers. First line is 1 (not zero)
    int line_number = 0;

    // Index of strings found so far
    int idx_strings_found = 0;
    while(getline(data_file, line) && idx_strings_found < num_str_to_find){
        line_number++;
        // Get starting values
        found_str = line.find(str_to_find[idx_strings_found]);
        if(found_str != string::npos){
            // Found string. Next string
            get_line_numbers(line, idx_vals[idx_strings_found], 2);
            // get_line_numbers(line, idx_starting_vals, 2);
            cout << "Starting values: " << idx_vals[idx_strings_found][0] << "\t" << idx_vals[idx_strings_found][1] << endl;
            idx_strings_found++;
            continue;
        }
    }

    //*******************************
    // Get the data
    //*******************************
    // Number of data_points

    cout << "Let's see what's left. Uhmmm" << endl;
    while(getline(data_file, line)){
        line_number++;
        cout << line << endl;
    }
    
    data_file.close();
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