#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

/**
 * @brief The Worker class
 *
 * Algoritmo da "bi-segmentação" -- este algoritmo é extensivamente usado na
 * identificação de objectos em imagens na área das Ciências dos Materiais, e
 * parte de dois valores L1 e L2, obtidos a partir de uma análise inicial da
 * imagem, tal como descrito em 1).
 * O algoritmo consiste no processamento de todos os voxels da imagem tal que:
 * se um voxel tiver uma cor inferior ao valor L1, o voxel fica com a cor preta;
 * senão, se o voxel tiver uma cor de valor maior ou igual a L1 e menor que L2,
 * o voxel fica com a cor cinzenta; senão, o voxel fica com a cor branca
 *
 */


/**
 * @brief Skeleton
 *
 * pipelining
 * Emitter -> farm<workers> -> collector
 */



/**
 * @brief The Emitter class
 */
class Emitter: public ff_node {


    Emitter(int ntask)
        :ntask(ntask)
    {}

    long ntask;

    int svc_init () {
        printf ("Work Start\n");
    }


    void *svc(void *)
    {
        long task = new task_t(ntask--);
        return (void*) task;
    }


};





/**
 * @brief The Worker class
 */
class Worker : public ff_node
{


    void *svc(void *task)
    {

    }


};


/**
 * @brief The Collector class
 */
class Collector: public ff_node
{
    void *svc(void *task)
    {
        printf ("Task=%d\n",(long)task);
        delete task;
        return GO_ON;
    }

    void svc_end()
    {
        printf ("Done!\n");
    }

};




int main()
{


}
