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

        static Eigen::MatrixXd model_function_value(Eigen::VectorXd x, 
                Eigen::VectorXd b){
            // INPUTS:
            //      x   :   Function input
            //      b   :   Parameters
            int num_output = 1;
            Eigen::VectorXd y(num_output);
            y(0) = exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
            return y;
        }
        
        static Eigen::MatrixXd model_function_gradient(Eigen::VectorXd x, 
            Eigen::VectorXd b){
                int num_parameters = 3;
                int num_input = 1;

                Eigen::MatrixXd gradient(num_parameters, num_input);

                gradient(0) = -x(0) * exp(-b(0) * x(0)) / (b(1) + b(2) * x(0));
                gradient(1) = - exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);
                gradient(2) = -x(0) * exp(-b(0) * x(0)) / pow(b(1) + b(2) * x(0), 2);

                return gradient;
            }

        Eigen::VectorXd error_function_value(
                Eigen::MatrixXd parameters, Eigen::MatrixXd* p_input_data, 
                Eigen::MatrixXd* p_output_data){
            // Eigen::MatrixXd *p_input_data = &input_data_;
            // Eigen::MatrixXd *p_output_data = &output_data_;

            int num_observations = this -> num_observations();
            // Residual vector            
            Eigen::VectorXd error_residual(num_observations);
            // Predicted output at each data point (changes at ever iteration)
            Eigen::VectorXd y_check;
            // Input at iteration k
            Eigen::VectorXd input_k;
            for(int i = 0; i < num_observations; i++){
                input_k = Eigen::VectorXd::Constant(1, (*p_input_data)(i));
                y_check = model_function_value(input_k, parameters);
                error_residual(i) = 
                    y_check(0) - 
                    (*p_output_data)(i);
            }    
            return error_residual;           
        }

        Eigen::MatrixXd error_function_jacobian(
                Eigen::MatrixXd parameters
                ){
            Eigen::MatrixXd *p_input_data = &input_data_;
            Eigen::MatrixXd *p_output_data = &output_data_;
            // Residual vector            
            Eigen::MatrixXd error_residual_jacobian
                (num_observations(), num_parameters());
                
            // Input at iteration k
            Eigen::VectorXd input_k(num_input());
            // Gradient at iteration k
            Eigen::MatrixXd gradient_k(num_output(), num_parameters());
            for(int i = 0; i < num_observations(); i++){
                for(int j = 0; j < num_input(); j++){
                    input_k(j) = (*p_input_data)(i, j);
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

    // Pointer to the parser class
    NIST_Parser* p_nist_parser = chwirut_problem.nist_parser();

    // **************************************************
    // Declarations and initialization
    // **************************************************
    // Get input data matrix
    Eigen::MatrixXd* p_input_matrix = chwirut_problem.p_input_data_matrix();
    // Get output data matrix
    Eigen::MatrixXd* p_output_matrix = chwirut_problem.p_output_data_matrix();

    // Vector containing residual values 
    Eigen::VectorXd error_vals;
    // Matrix contianing residual jacobian
    Eigen::MatrixXd error_jac;
    // Objective function value
    double obj_func_val;
    // Objective function gradient
    Eigen::VectorXd obj_func_grad(chwirut_problem.num_parameters());
    // Objective function Hessian (or approximate) matrix
    Eigen::MatrixXd obj_func_hess(chwirut_problem.num_parameters(), 
        chwirut_problem.num_parameters());
    // Vector containing the parameters to be optimized about
    Eigen::VectorXd params_k(chwirut_problem.num_parameters());

    // Search direction
    Eigen::VectorXd d_k(chwirut_problem.num_parameters());

    // Initialze parameters
    params_k = chwirut_problem.initial_parameters();

    // **************************************************
    // Optimization parameters
    // **************************************************
    int max_iterations = 1e2;
    // Stopping criterion
    double stop_tol = 1e-10;
    // Armijo rule parameters
    float alpha;
    float beta = 0.9;
    int max_iterations_armijo = 10;
    Eigen::VectorXd error_vals_temp;
    double obj_func_val_temp = -1;
    // Flag whether a solution is found or not
    int iterations_at_exit = -1;

    bool solution_found = false;
    bool max_iterations_reached = false;
    // Flag to output debugging details
    bool print_debug = false;
    // **************************************************
    // Optimization
    // **************************************************
    for(int ii = 0; ii < max_iterations; ii++){
        // Get error values and Jacobians
        error_vals = chwirut_problem.error_function_value(params_k, p_input_matrix, p_output_matrix);
        error_jac  = chwirut_problem.error_function_jacobian(params_k);

        // objective function
        obj_func_val = error_vals.transpose() * error_vals;
        obj_func_grad = error_vals.transpose() * error_jac;

        // Print obj. func. val
        if(ii%10 == 0 &&  print_debug){
            std::cout << "Obj. func. at " << 
                ii << "\t" <<obj_func_val << 
                ".\tObj. func. grad. norm: " <<
                obj_func_grad.norm() <<
                std::endl;
        }
        if(ii+1 == max_iterations){
            max_iterations_reached = true;
        }
        if(obj_func_grad.norm() < stop_tol){
            solution_found = true;
            iterations_at_exit = ii;
            break;
        }

        // Gauss-Newton Hessian approximation
        obj_func_hess = error_jac.transpose() * error_jac;

        d_k = - obj_func_hess.colPivHouseholderQr().solve(
                obj_func_grad);

        // Armijo rule
        for(int j = 0; j < max_iterations_armijo; j++){
            error_vals_temp = chwirut_problem.error_function_value(
                    params_k + pow(beta, j) * d_k, p_input_matrix, p_output_matrix);
            obj_func_val_temp = error_vals_temp.transpose() * error_vals_temp;
            if(obj_func_val_temp < obj_func_val){
                alpha = pow(beta, j);
                break;
            }
        }
        params_k += alpha * d_k;                
    }

    if(solution_found){
        std::cout << 
        "Possible solution found. Detilas:" << std::endl <<
        "\t" << "Residual        : " << obj_func_val << std::endl <<
        "\t" << "Obj. func. Jac. : " << obj_func_grad.norm() << std::endl <<
        "\t" << "Iterations      : " << iterations_at_exit << std::endl;
        std::cout << "Analysis: " << std::endl <<
        // "\t" << "Solution:\n" << params_k << "\n" <<std::endl <<
        // "\t" << "Certified solution:\n" << 
        //     chwirut_problem.certified_parameters() << "\n" <<std::endl <<
        "\t" << "Solution difference:\n" << chwirut_problem.certified_parameters() -
            params_k << "\n" << std::endl;
        
        std::cout << "\tComputed std:\n" << obj_func_hess.inverse().diagonal() << std::endl;
        std::cout << "\tCertified std:\n" << chwirut_problem.certified_parameters_std() << std::endl;
        std::cout << "\tDifference within bounds:" << std::endl;
        int temp_bool;
        for(int i = 0; i < chwirut_problem.num_parameters(); i++){
            temp_bool = 
                abs(chwirut_problem.certified_parameters()(i) - params_k(i)) <
                     chwirut_problem.certified_parameters_std()(i);
            std::cout << 
                temp_bool
                << "\n";
        }
    }
    if(max_iterations_reached){
        std::cout << "Maximum iterations reached: " <<
            max_iterations << std::endl;
    }
    std::cout << "Obj. func. grad.:\n" << error_jac.transpose() * error_vals << std::endl;
}   