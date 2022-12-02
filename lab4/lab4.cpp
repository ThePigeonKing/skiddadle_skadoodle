#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <random>

int main(int argc, char** argv)
{
    int procs = omp_get_num_procs();
    int threads = omp_get_max_threads();
    printf("======= Task2 =======\nAvailable procs = %d; threads = %d\n\n", procs, threads);

    int is_dynamic = omp_get_dynamic();
    printf("======= Task3 =======\nDynamic = %d\n\n", is_dynamic);

    double tickrate = omp_get_wtick();
    printf("======= Task4 =======\nOmp_tickrate = %.10lf\n\n", tickrate);

    int nested = omp_get_nested();
    int active_leves = omp_get_max_active_levels();
    printf("======= Task5 =======\nNESTED = %d; MAX_ACTIVE_LEVES = %d\n\n", nested, active_leves);

    std::cout << "======= Task6 =======\nSCHEDULE - " << omp_get_schedule << std::endl;

    omp_sched_t kind;
    int chunk_size;
    omp_get_schedule(&kind, &chunk_size);
    std::string schedule_msg;
    switch(kind)
    {
        case omp_sched_static:
        {
           schedule_msg = schedule_msg + "schedule=static, chunk_size="+std::to_string(chunk_size);
            break;
        }
        case omp_sched_dynamic:
        {
            schedule_msg = schedule_msg + "schedule=dynamic, chunk_size="+std::to_string(chunk_size);
            break;
        }
        case omp_sched_guided:
        {
            schedule_msg = schedule_msg + "schedule=guided, chunk_size="+std::to_string(chunk_size);
            break;
        }
        case omp_sched_auto:
        {
            schedule_msg = schedule_msg + "schedule=auto, chunk_size="+std::to_string(chunk_size);
            break;
        }
        default:
        {
            schedule_msg = schedule_msg + "schedule=monotonic, chunk_size="+std::to_string(chunk_size);
            break;
        }
    }
    std::cout << schedule_msg << std::endl;

    return 0;
}