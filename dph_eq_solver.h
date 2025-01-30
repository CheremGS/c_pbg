#pragma once
#include <iostream>
#include <random>

int** init_population(const int generation_size, const int characteristic_size, std::string mode = "random")
{
    int** generation = new int*[generation_size];
    for(int i = 0; i<generation_size; i++) 
        generation[i] = new int[characteristic_size];
    
    if (mode == "random")
    {
        std::random_device rand;  
        std::mt19937 gen(rand()); 
        std::uniform_int_distribution<> dis(1, 50); 

        for(int i=0; i<generation_size; i++)
        {
            for(int j=0; j<characteristic_size; j++)
            {
                generation[i][j] = int(dis(gen));
            }
        }
    }
    else // if(mode == "constant")
    {
        for(int i=0; i<generation_size; i++)
        {
            for(int j=0; j<characteristic_size; j++)
                {
                    generation[i][j] = 0;
                }
        }
    }
    // std::cout << "Init population complete" << std::endl;
    return generation;
}

int fitness_function(const int *ar, const int *eq_coefs)
{
    int diff = eq_coefs[0]*ar[0] + eq_coefs[1]*ar[1] + eq_coefs[2]*ar[2] + eq_coefs[3]*ar[3] + eq_coefs[4]; 
    diff = (diff < 0)?(-diff):(diff); 
    return diff;
}

double calc_prop_mult(const int * eq_coefs, const int eq_len)
{
    double mult_coef = 0;
    for(int i =0; i<eq_len; i++)
    {
        mult_coef += 1.0/eq_coefs[i];
    }
    return mult_coef;
}

void calc_surv_probs(double* fitness, double* surv_probs, const int* eq_coefs, const int gen_size, const int choronom_len)
{
    double coef_mult = calc_prop_mult(eq_coefs, choronom_len+1);
    for(int i = 0; i<gen_size; i++)
    {
        surv_probs[i] = 1/(coef_mult*fitness[i]);
    }
}

void couple_crossover(int* child, const int* parent1, const int* parent2, const int chronom_len)
{
    // optional toDo: add other crossover methods
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_int_distribution<>dist(1, chronom_len-2);
    int threshold_line = dist(gen);

    for(int i=0; i<chronom_len; i++)
    {
        child[i] = (i < threshold_line)?(parent1[i]):(parent2[i]);
    }
}

int** create_new_generation(int** generation, double* fitness, const int * eq_coefs, const int gen_size, const int choronom_len)
{
    int** new_generation = init_population(gen_size, choronom_len);
    double surv_probs[gen_size];
    // 1. calc probs (p[i] = 1/diffs(or fitness)[i]*(1/sum(eq_coefs)))
    calc_surv_probs(fitness, surv_probs, eq_coefs, gen_size, choronom_len);

    // 2. create sample of individ couples 
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::discrete_distribution<>dist(surv_probs, surv_probs+gen_size);

    for(int i=0; i<gen_size; i++)
    {
        // 2. create sample of individ couples 
        int index1 = dist(gen);
        int index2;
        do
        {
            index2 = dist(gen);
        }while(index2 == index1);
        // 3. crossover choromosom in each couple and create child
        int* parent1 = generation[index1];
        int* parent2 = generation[index2];
        couple_crossover(new_generation[i], parent1, parent2, choronom_len);
    }
    return new_generation;
}

void individ_mutation(int* individ, const int chronom_len)
{
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::discrete_distribution<>dist(individ, individ+chronom_len);

    int index1 = dist(gen);
    int index2;
    do
    {
        index2 = dist(gen);
    } while(index2 == index1);
    int temp = individ[index1];
    individ[index1] = individ[index2];
    individ[index2] = temp;
}

void generation_mutation(int** generation, const double mutation_prob, const int gen_size, const int chronom_len)
{
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution <> dist(0, 1);

    for(int i=0; i<gen_size; i++)
    {
        double mutation_chance = dist(gen);
        if(mutation_chance > mutation_prob)
            individ_mutation(generation[i], chronom_len);
    }
}

template <typename T2d>
void print_2darray(T2d** arr, int nrows, int ncols)
{
    for(int i=0; i<nrows; i++)
    {
        std::cout << i << " row: ";
        for(int j=0; j<ncols; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T1d>
void print_1darray(T1d* arr, int nelems)
{
    std::cout << "Array = [";
    for(int i=0; i<nelems; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "]" << std::endl;
}

void generation_fitness(int** generation, double* fitness, const int gen_size, const int* eq_coefs)
{
    for(int i=0; i<gen_size; i++)
    {
        fitness[i] = fitness_function(generation[i], eq_coefs);
    }
}

double mean_fitness(double* fitness, const int size)
{   
    double sum = 0;
    for(int i=0; i<size; i++)
    {
        sum += fitness[i];
    }
    return sum/size;
}

bool search_best_individ(int** generation, double* fitness, int* best_individ, const int gen_size, const int chronom_len)
{
    int max_fit = 1000; 
    int argmax_fit;
    bool opt_solution = false;
    for(int i=0; i<gen_size; i++)
    {
        if(fitness[i] < max_fit)
        {
            max_fit = fitness[i];
            argmax_fit = i;
        }
        if(max_fit == 0)
        {
            opt_solution = true;
            break;
        }
    }

    for(int i=0;i<chronom_len; i++)
    {
        best_individ[i] = generation[argmax_fit][i];
    }
    return opt_solution;
}

