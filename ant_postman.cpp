#include <iostream> 
#include "ant_postman.h"

// Проблема: если степень связности городов не высока, то много муравьев 
// не будут обходить все вершины 
// Решение (?) toDo: добавить проход муравья как по алгоритму Дийкстры (в случаях неполного графа)
// * добвить коэф испарения феромонов
int main()
{
    
    int num_iterations = 3;
    std::string data_path = "./postman_data_6p.txt";

    aco_opt(data_path, num_iterations);
    std::cout << "Program was complete" << std::endl; 
    return 0;
}