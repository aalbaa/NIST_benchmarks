#include "NIST_Problem.cpp"
#include "NIST_Parser.cpp"
#include <unsupported/Eigen/MatrixFunctions>


class Chwirut : public NIST_Problem{
    public:
        Chwirut(std::string file_name){
            nist_parser_ = new NIST_Parser(file_name);

            setDataFromParser(nist_parser_->data_vals(), nist_parser_->num_observations(), 
                nist_parser_->num_output(), nist_parser_->num_input(),
                 nist_parser_ -> num_params(), nist_parser_->params_certified_vals(), 
                 nist_parser_->params_starting_vals());

            // setModelFunction(model_function_value);
        };
        
        ~Chwirut(){
            delete nist_parser_;
        }

        NIST_Parser* nist_parser(){return nist_parser_;}

        Eigen::MatrixXd model_function_value(Eigen::VectorXd x, 
                Eigen::VectorXd b){
            // INPUTS:
            //      x   :   Function input
            //      b   :   Parameters
            Eigen::VectorXd y(num_output());
            y(0) = exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
            return y;
        }
        
        Eigen::MatrixXd model_function_gradient(Eigen::VectorXd x, 
            Eigen::VectorXd b){
                Eigen::MatrixXd gradient(num_parameters(), num_input());
                gradient(0) = -x(0) * exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
                gradient(1) = - exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);
                gradient(2) = -x(0) * exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);

                return gradient;
            }

        Eigen::VectorXd error_function_value(
                Eigen::MatrixXd input_data, 
                Eigen::MatrixXd output_data,
                Eigen::MatrixXd parameters){
            int num_observations = this -> num_observations();
            // Residual vector            
            Eigen::VectorXd error_residual(num_observations);
            // Predicted output at each data point (changes at ever iteration)
            Eigen::VectorXd y_check;
            // Input at iteration k
            Eigen::VectorXd input_k;
            for(int i = 0; i < num_observations; i++){
                input_k = Eigen::VectorXd::Constant(1, input_data(i));
                y_check = model_function_value(input_k, parameters);
                error_residual(i) = 
                    y_check(0) - 
                    output_data(i);
            }    
            return error_residual;           
        }

        Eigen::MatrixXd error_function_jacobian(
                Eigen::MatrixXd input_data, 
                Eigen::MatrixXd output_data,
                Eigen::MatrixXd parameters){
            // Residual vector            
            Eigen::MatrixXd error_residual_jacobian
                (num_observations(), num_parameters());
                
            // Input at iteration k
            Eigen::VectorXd input_k(num_input());
            // Gradient at iteration k
            Eigen::MatrixXd gradient_k(num_output(), num_parameters());
            for(int i = 0; i < num_observations(); i++){
                for(int j = 0; j < num_input(); j++){
                    input_k(j) = input_data(i, j);
                }
                gradient_k = model_function_gradient(input_k, parameters);
                for(int j = 0; j < num_parameters(); j++){
                    error_residual_jacobian(i, j) = gradient_k(j, 0);
                }
            }    
            return error_residual_jacobian;           
        }


    private:
        NIST_Parser* nist_parser_; 
};


int main(){
    std::string file_name = "Chwirut2_dat.txt";
    
    Chwirut chwirut_problem(file_name);

    std::cout << "Number of observations: " <<
         chwirut_problem.nist_parser() -> num_observations() 
         << std::endl;

    std::cout << "Number of parameters: " << chwirut_problem.num_parameters()
         << std::endl;

    // Try out the function
    NIST_Parser* p_nist_parser = chwirut_problem.nist_parser();

    std::cout << "Certified values: " << std::endl;
    std::cout << p_nist_parser -> params_certified_vals()[0][0] << std::endl;
    std::cout << chwirut_problem.certified_parameters() << std::endl;

    std::cout << "Testing function: " << std::endl;
    std::cout << Eigen::VectorXd::Zero(1) << std::endl;
    
    Eigen::MatrixXd error_vals;
    Eigen::MatrixXd error_jac;

    error_vals = chwirut_problem.error_function_value(
        chwirut_problem.input_data_matrix(), 
        chwirut_problem.output_data_matrix(),
        chwirut_problem.initial_parameters()
        );
    error_jac = chwirut_problem.error_function_jacobian(
        chwirut_problem.input_data_matrix(), 
        chwirut_problem.output_data_matrix(),
        chwirut_problem.initial_parameters()
        );


    // Eigen::MatrixXd (*error_func_vals_lambda)(Eigen::VectorXd) = [chwirut_problem](Eigen::VectorXd params)
    //     {return chwirut_problem.error_function_value(
    //     chwirut_problem.input_data_matrix(), 
    //     chwirut_problem.output_data_matrix(),
    //     params
    //     );};
    
    std::cout << "Errors:\n" << 
        error_vals << std::endl << std::endl;
    std::cout << "Errors jacobians:\n" << 
        error_jac << std::endl << std::endl;

    std::cout << "Residual:\t" << error_vals.transpose()*error_vals 
        << std::endl;
    std::cout << "Obj. func. grad.:\n" << error_jac.transpose() * error_vals << std::endl;

}   