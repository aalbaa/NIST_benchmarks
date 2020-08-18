#include "NIST_Parser.cpp"
#include <Eigen/Dense>
#include <string>
#include <iostream>

class NIST_Problem{
    public:
        NIST_Problem(){};

        void setModelFunction(Eigen::MatrixXd (*func_ptr)(Eigen::MatrixXd x)){
            func_ptr_ = func_ptr;
        }

        Eigen::MatrixXd model_function(Eigen::MatrixXd x){
            if(func_ptr_){
                return func_ptr_(x);
            }else{
                throw;
            }
        }

        int num_observations(){return num_observations_;}
        
        int num_output(){return num_output_;}

        int num_input(){return num_input_;}

        void setNumberOfObservations(int num_observations){
            num_observations_ = num_observations;
        }
        void setNumberOfOutputs(int num_output){
            num_output_ = num_output;
        }
        void setNumberOfInputs(int num_input){
            num_input_ = num_input;
        }
        void setInputMatrix(double** data_vals){

            // Function to convert the double array into matrix. 
            // This function returns the input values

            // Matrix of the output values
            Eigen::MatrixXd input_matrix(num_observations_, num_input_);
            for(int i = 0; i < num_observations_; i++){
                // First, the outputs
                for(int j = 0; j < num_input_; j++){
                    input_matrix(i, j) = data_vals[i][num_output_ + j];
                }
            }
            input_data_ = input_matrix;
        }

        void setOutputMatrix(double** data_vals){
            // Function to convert the double array into matrix. 
            // This function returns the output values

            // Matrix of the output values
            Eigen::MatrixXd output_matrix(num_observations_, num_output_);
            for(int i = 0; i < num_observations_; i++){
                // First, the outputs
                for(int j = 0; j < num_output_; j++){
                    output_matrix(i, j) = data_vals[i][j];
                }
            }
            output_data_ = output_matrix;
        }

        void setDataFromParser(double** data_vals, int num_observations, 
            int num_outputs, int num_inputs){
                setNumberOfObservations(num_observations);
                setNumberOfOutputs(num_outputs);
                setNumberOfInputs(num_inputs);
                setInputMatrix(data_vals);
                setOutputMatrix(data_vals);
        }

        Eigen::MatrixXd input_data_matrix(){return input_data_;}
        
        double input_data_element(int row, int col){
            return input_data_(row, col);
        }

        Eigen::MatrixXd output_data_matrix(){return output_data_;}
        double output_data_element(int row, int col){
            return output_data_(row, col);
        }


    private:
        // Data inputs
        Eigen::MatrixXd input_data_;
        Eigen::MatrixXd output_data_;
        
        Eigen::MatrixXd (*func_ptr_)(Eigen::MatrixXd input) = nullptr;

        int num_observations_ = -1;
        int num_output_ = -1;
        int num_input_ = -1;
};


// Model function
Eigen::MatrixXd model_function_value(Eigen::MatrixXd x){
    return x.transpose()*x;
}

int main(){

    std::string file_name = "Chwirut2_dat.txt";
    NIST_Parser nist_parser(file_name);

    NIST_Problem nist_problem;
    nist_problem.setModelFunction(model_function_value);

    nist_problem.setDataFromParser(nist_parser.data_vals(), nist_parser.num_observations(), 
            nist_parser.num_output(), nist_parser.num_input());
}


