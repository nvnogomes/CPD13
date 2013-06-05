#include <iostream>
#include <map>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

#include "readImage.c"

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

    // shared image buffer
    char* image;


/**
 * @brief The Emitter class
 *
 * distribute strips of the image
 */
class Emitter: public ff_node {

    long sideSize;
    int offSet;


    Emitter(int size)
        :sideSize(size), offSet(0)
    {}



    int svc_init () {
        printf ("Work Start\n");
    }


    void *svc(void *)
    {
        long task = new task_t(offSet + sideSize);
        offSet = sideSize;
        return (void*) task;
    }


};





/**
 * @brief The Worker class
 *
 * creates a histogram of the strip received
 */
class Worker : public ff_node
{
    std::map<char,int> histogram;

    void svc_init()
    {
    }


    void *svc(void *task)
    {

        unsigned char* strip = (unsigned char*) task;
        for( int currentIndex = 0; currentIndex < imageSize; currentIndex++ )
        {
            unsigned char currentVoxel = strip[currentIndex];
            histogram[currentVoxel]++;
        }
        return (void *) histogram;
    }
};


/**
 * @brief The Collector class
 *
 * reduce all the histograms
 * calculates L1 and L2
 */
class Collector: public ff_node
{
    std::map<char,int> global_histogram;

    void *svc(void *task)
    {
        std::map<char,int> partial_histogram = (std::map<char,int>*) task;
        std::map<char,int>::const_iterator it = partial_histogram.begin();
        for( ; it != it_end; ++it)
        {
           global_histogram.insert(*it);
        }
        delete task;
        return GO_ON;
    }

    void svc_end()
    {
        // calculate L1 and L2 ?
        printf ("Done!\n");
    }

};



/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if( argc != 2 )
    {
        perror("USAGE: histogram #workers imageSize");
    }

    int nWorkers = atoi(argv[1]);
    int imageSize = atoi(argv[2]);

    ff_farm<> farm;
    std::vector<ff_node*> workers;
    for(int i=0; i < nWorkers; ++i)
    {
        workers.push_back(new Worker);
    }

    unsigned char *image = readImage("cube_"+ size +".mat");

    farm.add_workers(workers);
    farm.add_emitter(new Emitter(image, imageSize));
    farm.add_collector(new Collector);


}
