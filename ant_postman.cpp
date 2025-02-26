#include <iostream> 
#include "ant_postman.h"

int main()
{
    int num_iterations = 100;
    std::string data_path = "./postman_data.txt";
    // toDo: check weighted path len calcs
    // Ant ant_colony[POINT_NUMBER];
    // init_ants_points(ant_colony);
    // int** point_distance_matrix = init_dyn_matrix<int>();
    // read_txt_matrix(point_distance_matrix);

    // double** pheromona_matrix = init_dyn_matrix<double>();
    // init_phero_values(point_distance_matrix, pheromona_matrix);
    
    // ant_colony_run(ant_colony, pheromona_matrix, point_distance_matrix, RUN_NUMBER);
    aco_opt(data_path, num_iterations);
    std::cout << "Program was complete" << std::endl; 
    return 0;
}