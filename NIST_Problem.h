#include "NIST_Parser.cpp"
#include <Eigen/Dense>
#include <string>
#include <iostream>

// using namespace Eigen;

class NIST_Problem{
    public:
        // Constructor without setting file_name
        NIST_Problem();
        // Constructor with a file_name
        NIST_Problem(std::string file_name);

        // file_name setter
        void setFileName(std::string file_name);
        // file_name getter
        std::string file_name();

        void setModelFunction(Eigen::MatrixXd (*func_ptr)(Eigen::MatrixXd input));

        // // Model function. x is the input
        // Eigen::MatrixXd modelFunction(Eigen::MatrixXd );
    private:
        // Data file_name 
        std::string file_name_;

        // Data inputs
        Eigen::MatrixXd input_data_;
        Eigen::MatrixXd output_data_;
        
        Eigen::MatrixXd (*func_ptr_)(Eigen::MatrixXd input);
};
