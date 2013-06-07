#include <iostream>
#include <map>
#include <string>
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
 * @todo
 *  1. read image
 *  2. decide structure
 *  2.1 passing map by reference
 *  3. pipeline
 */


/**
 * @brief Skeleton
 *
 * pipelining
 * Emitter -> farm<workers> -> collector
 *
 */


// shared image buffer
unsigned char* imageBuffer;



/**
 * @brief The Emitter class
 *
 * distribute strips of the image
 */
class Emitter: public ff::ff_node {

private:
    int sideSize;
    int offSet;

public:
    Emitter(int size);

    int svc_init ();
    void *svc(void *);
};

Emitter::Emitter(int size)
    : sideSize(size), offSet(0)
{}

int Emitter::svc_init()
{
    printf ("Work Started\n");
    return 0;
}

void* Emitter::svc(void *)
{
    // task?? how to create a task to the next stage??
    int task = offSet + sideSize;
    offSet = sideSize;
    return (void*) task;
}


/**
 * @brief The Worker class
 *
 * creates a histogram of the strip received
 */
class Worker: public ff::ff_node
{

private:
    std::map<char,int> histogram;

public:
    int svc_init();
    void *svc(void *task);
};

int Worker::svc_init()
{
    return 0;
}

void* Worker::svc(void* task)
{
    unsigned char* strip = (unsigned char*) task;
    std::cout << "strip" << std::endl;
    int imageSize = strlen((char*)strip);
    for( int currentIndex = 0; currentIndex < imageSize; currentIndex++ )
    {
        unsigned char currentVoxel = strip[currentIndex];
        histogram[currentVoxel]++;
    }
    return (void *) &histogram;
}


/**
 * @brief The Collector class
 *
 * reduce all the histograms
 * calculates L1 and L2
 */
class Collector : public ff::ff_node
{
private:
    std::map<char,int> global_histogram;

public:
    void *svc(void *task);
    void svc_end();
    void appendMap(std::map<char,int> map);

};

void* Collector::svc(void *task)
{
    std::map<char,int> *partial_histogram = (std::map<char,int>*) task;

    appendMap( *partial_histogram );

    return GO_ON;
}

void Collector::appendMap(std::map<char,int> map)
{
    std::map<char,int>::const_iterator it = map.begin();
    for( ; it != map.end(); ++it)
    {
       global_histogram.insert(*it);
    }
}

void Collector::svc_end()
{
    // calculate L1 and L2 ?
    printf ("Done!\n");
}


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if( argc != (2 + 1) )
    {
        std::cerr << "USAGE: histogram #workers image_file ("
                  << argc << ")" << std::endl;
        exit(1);
    }

    int nWorkers = atoi(argv[1]);
    int imageSize = atoi(argv[2]);

    ff::ff_farm<> farm;
    std::vector<ff::ff_node*> workers;
    for(int i=0; i < nWorkers; ++i)
    {
        workers.push_back(new Worker);
    }

    imageBuffer = readImage( argv[2] );

    farm.add_emitter(new Emitter(imageSize));
    farm.add_workers(workers);
    farm.add_collector(new Collector);

    return 0;
}
