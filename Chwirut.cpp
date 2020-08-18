#include "NIST_Problem.cpp"
#include "NIST_Parser.cpp"
#include <unsupported/Eigen/MatrixFunctions>


class Chwirut : public NIST_Problem{
    public:
        Chwirut(std::string file_name){
            nist_parser_ = new NIST_Parser(file_name);

            setDataFromParser(nist_parser_->data_vals(), nist_parser_->num_observations(), 
                nist_parser_->num_output(), nist_parser_->num_input(),
                 nist_parser_ -> num_params());

            setModelFunction(model_function_value);
        };
        
        ~Chwirut(){
            delete nist_parser_;
        }
        NIST_Parser* nist_parser_; 
    private:
        static double model_function_value(Eigen::VectorXd x, 
                Eigen::VectorXd b){
            // INPUTS:
            //      x   :   Function input
            //      b   :   Parameters

            return exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
        }

        Eigen::MatrixXd model_function_gradient(Eigen::VectorXd x, 
            Eigen::VectorXd b){
                Eigen::MatrixXd gradient(num_parameters(), 1);
                gradient(0) = -x(0) * exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
                gradient(1) = - exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);
                gradient(2) = -x(0) * exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);

                return gradient;
            }
};

int main(){
    std::string file_name = "Chwirut2_dat.txt";
    
    Chwirut chwirut_problem(file_name);

    std::cout << "Number of observations: " <<
         chwirut_problem.nist_parser_ -> num_observations() 
         << std::endl;

    std::cout << "Number of parameters: " << chwirut_problem.num_parameters()
         << std::endl;
}