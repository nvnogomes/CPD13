#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

/**
 * @brief The Worker class
 *
 * Construção de um histograma -- para todas as cores possíveis (do preto ao
 * branco, i.e. do valor 0 ao 255) pretende-se contar quantos voxels na imagem
 * apresentam essa cor.
 * O objectivo é a construção de um histograma de modo a identificar dois
 * valores de tom de cinzento L1 e L2 que melhor permitam distinguir os dois
 * tipos de materiais numa amostra. Assim, o valor L1 representa um aglomerado
 * de voxels com cores mais próximas do preto correspondente a um tipo de
 * material, e o L2 do branco, correspondente a outro tipo de material.
 * O objectivo da obtenção dos dois valores é permitir transformar a imagem
 * original numa outra em que existe um maior contraste entre os dois tipos de
 * material, transformando todos os voxels em voxels apenas com cores preta
 * ou branca, usando os dois algoritmos seguintes.
 */


/**
 * @brief Skeleton
 *
 * pipelining
 * Emitter -> farm<workers> -> collector
 *
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
