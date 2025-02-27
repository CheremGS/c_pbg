# pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>

const bool DEBUG = true;

constexpr int POINT_NUMBER = 6;
constexpr int ANT_COLONY_SIZE = POINT_NUMBER;

const float BETA = 1.0;
const float ALPHA = 1.0;
const float BASE_PHERO_VALUE = 0.5;
const float EVAPORATION_RATE = 0.1;

struct Ant {
    int ant_path[POINT_NUMBER];
    int path_len = 1;
    double weight_path_len=0;
    bool loop_path = false;
};

template <typename T>
T** init_dyn_matrix();
void init_ants_points(Ant* ant_colony);
void init_phero_values(int** dist_matrix, double** pheros_matrix);
void read_txt_matrix(int** matrix, const std::string data_file);
int ant_step(int* distance, const double* pheromona, const int* blocked_points);
void ant_run(Ant& ant, double** path_pheromona, int** point_distances);
void ant_colony_run(Ant ant_colony[ANT_COLONY_SIZE], double** path_pheromona, int** point_distances);
void aco_opt(const std::string path_data, const int num_iterations);
void pheromona_update(double** pheromona_matrix, Ant* ant_colony);

std::ostream& operator<<(std::ostream& out, Ant ant);
template <typename T1d>
void print_1darray(T1d* arr, int nelems);
template <typename T2d>
void print_2darray(T2d** arr, int nrows, int ncols);


void aco_opt(const std::string path_data, const int num_iterations){
    int** point_distance_matrix = init_dyn_matrix<int>();
    double** pheromona_matrix = init_dyn_matrix<double>();
    read_txt_matrix(point_distance_matrix, path_data);
    init_phero_values(point_distance_matrix, pheromona_matrix);

    if(DEBUG){
        std::cout << "Init values in distance matrix:" << '\n';
        print_2darray(point_distance_matrix, POINT_NUMBER, POINT_NUMBER);
        std::cout << "Init values in pheromona matrix:" << '\n';
        print_2darray(pheromona_matrix, POINT_NUMBER, POINT_NUMBER);
    }

    int* best_path;
    double min_path_len = __DBL_MAX__;
    
    for(int i_iter=0; i_iter<num_iterations; i_iter++){
        std::cout << '\n' << i_iter << " iteration:" << '\n';
        Ant ant_colony[POINT_NUMBER];
        init_ants_points(ant_colony);
        ant_colony_run(ant_colony, pheromona_matrix, point_distance_matrix);
        pheromona_update(pheromona_matrix, ant_colony);
        for(int i_ant=0; i_ant < ANT_COLONY_SIZE; i_ant++){
            if(ant_colony[i_ant].loop_path)
                if(ant_colony[i_ant].weight_path_len < min_path_len){
                    std::cout << "New opt path was found: " << int(min_path_len) << " -> " ;
                    std::cout << int(ant_colony[i_ant].weight_path_len) << '\n';
                    best_path = ant_colony[i_ant].ant_path;
                    min_path_len = ant_colony[i_ant].weight_path_len;
                }
            else 
                continue;
        }
    }
}

void pheromona_update(double** pheromona_matrix, Ant* ant_colony){
    for(int i_ant=0; i_ant<ANT_COLONY_SIZE; i_ant++){
        if(ant_colony[i_ant].loop_path){
            double delta_pheros = double(POINT_NUMBER)/ant_colony[i_ant].weight_path_len;
            for(int i_edge=0; i_edge<POINT_NUMBER-1; i_edge++){
                pheromona_matrix[i_edge][i_edge+1] += delta_pheros;
            }
        }
        else
            continue;
    }
    if(DEBUG){
        std::cout << "Update pheromona matrix" << '\n'; 
        print_2darray(pheromona_matrix, POINT_NUMBER, POINT_NUMBER);
    }
        
}

void ant_colony_run(Ant ant_colony[ANT_COLONY_SIZE], double** path_pheromona, int** point_distances){
    for(int i_ant=0; i_ant < ANT_COLONY_SIZE; i_ant++){
        std::cout << " - - - - - Ant " << i_ant << " run - - - - - " << '\n';
        ant_run(ant_colony[i_ant], path_pheromona, point_distances);
        std::cout << ant_colony[i_ant];
    }
}

void ant_run(Ant& ant, double** path_pheromona, int** point_distances){
    // ant struct will contain new data after run  
    for(int i=0; i<POINT_NUMBER; i++){
        if(ant.path_len == POINT_NUMBER){
            ant.loop_path = true;
            break;
        }
        int ant_loc_paths[POINT_NUMBER];
        for(int n=0; n< POINT_NUMBER; n++)
            ant_loc_paths[n] = point_distances[ant.ant_path[i]][n];
        double available_path_len = 0;
        double* ant_loc_pheros = path_pheromona[ant.ant_path[i]];
        if(DEBUG){
            std::cout << "Paths from current point (" << ant.ant_path[i] << ") - ";
            print_1darray(ant_loc_paths, POINT_NUMBER);
        }
        // check already visited points and zeros path to them; visited points probs will be equal zeros 
        for(int k=0; k<ant.path_len; k++){
            ant_loc_paths[ant.ant_path[k]] = 0;
        }
        if(DEBUG){
            std::cout << "Pheromona from current point - ";
            print_1darray(ant_loc_pheros, POINT_NUMBER);
            std::cout << "Unvisited paths from current point - ";
            print_1darray(ant_loc_paths, POINT_NUMBER);
        }
        // check up for new unvisited nodes
        for(int k = 0; k<POINT_NUMBER; k++)
            available_path_len += ant_loc_paths[k];
        
        if(available_path_len > 0){
            int ant_next_point = ant_step(ant_loc_paths, ant_loc_pheros, ant.ant_path);
            ant.ant_path[i+1] = ant_next_point;
            ant.weight_path_len += ant_loc_paths[ant_next_point];
            ant.path_len++;
            if(DEBUG){
                std::cout << "Ant choose point " << ant_next_point << std::endl;
                std::cout << "Ant path: ";
                print_1darray(ant.ant_path, POINT_NUMBER);
            }
        }
        else
            break;
    }
}

int ant_step(int* distance, const double* pheromona, const int* blocked_points){
    double probs[POINT_NUMBER];
    double sum_weights = 0; 
    for(int i=0; i < POINT_NUMBER; i++) {
        probs[i] = (distance[i]>0)?(pow(1/(double)distance[i], BETA)*pow(pheromona[i], ALPHA)):(0);
        sum_weights += probs[i];
    }
    for(int i=0; i < POINT_NUMBER; i++)
        probs[i] /= sum_weights;
    if(DEBUG){
        std::cout << "Step point probabilities: ";
        print_1darray(probs, POINT_NUMBER);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution <> dist(probs, probs+POINT_NUMBER);
    int next_point = dist(gen); 
    return next_point;
}

template <typename T>
T** init_dyn_matrix() {
    T** matrix = new T*[POINT_NUMBER];
    for(int i = 0; i<POINT_NUMBER; i++) 
        matrix[i] = new T[POINT_NUMBER];
    // if exclude code below, first element will be equal 1.46602e+13
    for(int i=0; i<POINT_NUMBER; i++) {
        for(int j=0; j<POINT_NUMBER; j++)
                matrix[i][j] = 0;
    }
    return matrix;
}

void init_ants_points(Ant* ant_colony) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution <> point_gen(0, POINT_NUMBER-1);
    for(int i=0; i<ANT_COLONY_SIZE; i++){
        ant_colony[i].ant_path[0] = point_gen(gen);
        for(int i_p = 1; i_p<POINT_NUMBER; i_p++)
            ant_colony[i].ant_path[i_p] = -1;
    }  
}

void init_phero_values(int** dist_matrix, double** pheros_matrix){
    for(int i=0; i<POINT_NUMBER; i++)
        for(int j=0; j<POINT_NUMBER; j++)
            if(dist_matrix[i][j] > 0)
                pheros_matrix[i][j] = BASE_PHERO_VALUE;
}

void read_txt_matrix(int** matrix, const std::string data_file = "./postman_data.txt") {
    std::ifstream file_stream{data_file};
    std::string filedata;
    if(!file_stream)
        std::cerr << "Error file read process!" << std::endl;
    else {
        std::string strInput{};
        int index = 0;
        while(file_stream >> strInput){
            matrix[index/POINT_NUMBER][index%POINT_NUMBER] = std::stoi(strInput);
            index++;
        }
    }
}

std::ostream& operator<<(std::ostream& out, Ant ant){
    if(ant.loop_path) {
        out << "An ant has passed the path with all points - ";
        print_1darray(ant.ant_path, ant.path_len);
        out << "Path length = " << ant.weight_path_len << std::endl; 
    }
    else {
        out << "An ant didnt go around all the points - ";
        print_1darray(ant.ant_path, ant.path_len);
    }
    return out;
}

template <typename T2d>
void print_2darray(T2d** arr, int nrows, int ncols){
    for(int i=0; i<nrows; i++){
        std::cout << i << " row: ";
        for(int j=0; j<ncols; j++)
            std::cout << arr[i][j] << " ";
        std::cout << std::endl;
    }
}

template <typename T1d>
void print_1darray(T1d* arr, int nelems){
    std::cout << "Array = [";
    for(int i=0; i<nelems; i++)
       std::cout << arr[i] << " ";
    std::cout << "]" << std::endl;
}