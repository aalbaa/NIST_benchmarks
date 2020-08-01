#include <iostream>
#include <fstream> // Might not be necessary later
#include <string>
#include <sstream>
#include <map>

using namespace std;


// Number of integer parameters
const int kNum_intParams = 9;
// Array of string names of the integer parameters
const string kIntParams_arr[] = {"Starting Values", "Certified Values", "Data ", 
" Response", " Predictor", " Observations", " Parameters", 
"Degrees of Freedom", "Number of Observations"};

// Array of string names of the residual parameters
const string kDoubleParams_arr[] =  {"Residual Sum of Squares", "Residual Standard Deviation"};
// Number of the 'double' type parameters (residuals)
const int kNum_doubleParams = 2;


class NIST_Parser{

    public:
        NIST_Parser(string file_name){
            file_name_ = file_name;

            // Number of integer parameters
            num_intParams_ = kNum_intParams;

            // Number of double parameters
            num_doubleParams_ = kNum_doubleParams;

            // Array of residual errors
            residual_arr_ = new double[num_doubleParams_];

            // Line numbers of the integer parameters
            line_numbers_intParams_ = new int[num_intParams_];
            // Initialize to -1
            for(int i=0; i<num_intParams_; i++){
                line_numbers_intParams_[i] = -1;
            }
            
        }
        
        ~NIST_Parser(){
            // Close file if it's open
            if(file_stream_.is_open()){
                file_stream_.close();
            }

            // Delete dynamic arrays
            delete[] line_numbers_intParams_;
            delete[] residual_arr_;

            if(data_parsed_){
                // Delete the 2D dynamic array 
                for(int i = 0; i < num_params_; i++){
                    delete[] params_starting_vals_[i];
                    delete[] params_certified_vals_[i];
                }
                delete[] params_starting_vals_;
                delete[] params_certified_vals_;

                for(int i = 0; i < num_observations_; i++){
                    delete[] data_vals_[i];
                }
                delete[] data_vals_;
            }
        }

        void OpenFile(string file_name){
            // Set the new name
            file_name_ = file_name;
            if(!file_stream_.is_open()){
                file_stream_.open(file_name_);
            }
        }

        void CloseFile(){
            if(file_stream_.is_open()){
                file_stream_.close();
            }
        }
        void OpenFile(){
            return file_stream_.open(file_name_);
        }

        // TEMPORARY! This function will not be used later
        int* line_numbers_intParams(){return line_numbers_intParams_;};

        double* residual_arr(){
            if(!param_lines_parsed_){
                ParseParameterLines(); 
            }
            return residual_arr_;
        }

        double** params_starting_vals(){
            if(!data_parsed_){
                ParseData();
            }

            return params_starting_vals_;
        }

        double** params_certified_vals(){
            if(!data_parsed_){
                ParseData();
            }

            return params_certified_vals_;
        }

        double** data_vals(){
            if(!data_parsed_){
                ParseData();
            }

            return data_vals_;
        }
        void ParseParameterLines(){
            // This function only gets the parameter lines (the integer parameters)

            if(param_lines_parsed_){
                return;
            }
            if(!file_stream_.is_open()){
                OpenFile();
            }

            //  size_t variable keeping track of strings to find
            size_t found_str;

            // Input lines
            string input_line; 

            // Integer keeping track of line numbers. First line is 1 (not zero)
            int line_number = 0;
            // Number of strings found so far
            int num_intParams_found = 0;
            // Number of 'double parameters' found
            int num_doubleParams_found = 0;
            while(getline(file_stream_, input_line) && num_intParams_found < num_intParams_){
                line_number++;
                // Get starting values
                found_str = input_line.find(kIntParams_arr[num_intParams_found]);
                if(found_str != string::npos){
                    GetLineNumbers(input_line, line_numbers_intParams_ + num_intParams_found, 1);
                    num_intParams_found++;
                    continue;
                }
                // Otherwise, look for the residual values
                found_str = input_line.find(kDoubleParams_arr[num_doubleParams_found]);
                if(found_str != string::npos){
                    // Add value to the array address so to store parameters in the appropriate element locations
                    GetLineNumbers(input_line,  residual_arr_ + num_doubleParams_found, 1);
                    num_doubleParams_found++;
                    continue;
                }
            }
            
            // Number of parameters
            num_params_ = line_numbers_intParams_[6];
            // Number of inputs (predictor)
            num_input_ = line_numbers_intParams_[4];
            // Number of outputs (response)
            num_output_ = line_numbers_intParams_[3];
            // Number of observations
            num_observations_ = line_numbers_intParams_[5];

            // 'Starting values' first line
            line_starting_vals_ = line_numbers_intParams_[0];            

            // 'data values' first line
            line_data_ = line_numbers_intParams_[2];            

            // Flip the flag if parameters successfully read
            param_lines_parsed_ = true;

            // Close file
            file_stream_.close();
        }

        void ParseData(){
            // This function parses the starting and certified parameters, 
            // and the data (x,y)
            
            
            if(!param_lines_parsed_) ParseParameterLines();

            // If file is open, close it and re-open
            if(file_stream_.is_open()) CloseFile();
            OpenFile();
            
            // Starting values (there are two starting values).
            params_starting_vals_ = new double*[num_params_];
            // Parametr certified value (first column) and standard deviation (second column)
            params_certified_vals_ = new double*[num_params_];
                    
            // Construct the 2D dynamic array 
            for(int i = 0; i < num_params_; i++){
                params_starting_vals_[i]  = new double[2];
                params_certified_vals_[i] = new double[2];
            }
            
            // Construct the 2D array for data
            data_vals_ = new double*[num_observations_];
            for(int i=0; i<num_observations_; i++){
                data_vals_[i] = new double[num_input_ + num_output_];
            }

            // Temporary array that's passed to the line parser
            double params_temp_vals[4];

            int line_number = 0;
            string input_line;
            while(getline(file_stream_, input_line)){
                line_number++;

                // Get the starting and certified values
                if(line_number >= line_starting_vals_ && line_number - line_starting_vals_ < num_params_){
                    // Get the starting values first
                    GetLineNumbers(input_line, params_temp_vals, 4);
                    for(int i = 0; i < 2; i++){
                        params_starting_vals_[line_number-line_starting_vals_][i] = params_temp_vals[i];
                        params_certified_vals_[line_number-line_starting_vals_][i] = params_temp_vals[i+2];
                    }
                    continue;
                }
                if(line_number >= line_data_ && line_number - line_data_ < num_observations_){
                    // Get the data values first
                    GetLineNumbers(input_line, data_vals_[line_number-line_data_], num_input_ + num_output_);
                    continue;
                }
            }

            data_parsed_ = true;
        }
        int num_params(){
            if(!param_lines_parsed_) ParseParameterLines();
            return num_params_;
        }

        int num_input(){
            if(!param_lines_parsed_) ParseParameterLines();
            return num_input_;
        }
        
        int num_output(){
            if(!param_lines_parsed_) ParseParameterLines();
            return num_output_;
        }

        int num_observations(){
            if(!param_lines_parsed_) ParseParameterLines();
            return num_observations_;
        }
    private:
        void GetLineNumbers(string str, double * vals, int num_vals){
            // Get a line number from a string.
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
        
        void GetLineNumbers(string str, int * idx, int num_ints){
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

        // Input file stream
        ifstream file_stream_;

        // Filename
        string file_name_ = "";

        // Number of parameters
        int num_params_;
        // Number of inputs (predictor)
        int num_input_;
        // Number of outputs (response)
        int num_output_;
        // Number of observations
        int num_observations_;

        // Line number where 'starting values' data starts
        int line_starting_vals_;

        // Line number where data starts
        int line_data_;

        // ***************************
        // Flags
        // Flag indicating whether parameters have been read or not
        bool param_lines_parsed_ = false;
        bool data_parsed_ = false;



        // ***************************
        // Number of integer parameters
        int num_intParams_;
        // Indices of line numbers
        int* line_numbers_intParams_;

        // Number of double parameters
        int num_doubleParams_;
        double* residual_arr_;
        
        // Starting values (there are two starting values).
        double** params_starting_vals_;
        // Parametr certified value (first column) and standard deviation (second column)
        double** params_certified_vals_;

        // Data 2D array
        double** data_vals_;
};

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

