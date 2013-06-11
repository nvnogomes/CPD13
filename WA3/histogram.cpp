#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <ff/allocator.hpp>

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
static unsigned char* imageBuffer;
static int imageSize;

static std::map<char,int> histogram;




/**
 * @brief The Emitter class
 *
 * distribute strips of the image
 */
class Emitter: public ff::ff_node {

private:
    int offSet;
    int repetitionsLeft;

public:
    Emitter();

    int svc_init ();
    void *svc(void *);
    void svc_end ();

};

/**
 * @brief Emitter::Emitter
 * @param size
 * @param filename
 */
Emitter::Emitter()
    : offSet(0), repetitionsLeft(imageSize)
{
}

/**
 * @brief Emitter::svc_init
 * @return
 */
int Emitter::svc_init()
{
    return 0;
}

/**
 * @brief Emitter::svc
 * @return
 */
void* Emitter::svc(void *)
{
    while(repetitionsLeft >= 0)
    {
        char *imageStrip = (char *) malloc((imageSize+1)*sizeof(char));

        int stripStart = repetitionsLeft + imageSize;

        memcpy( imageStrip, &imageBuffer[stripStart], imageSize );
        imageStrip[imageSize] = '\0';

        ff::ff_node::ff_send_out((void *) imageStrip);
        repetitionsLeft--;
    }
    ff_send_out(EOS);
    return NULL;
}


/**
 * @brief Emitter::svc_end
 */
void Emitter::svc_end()
{
}



/**
 * @brief The Worker class
 *
 * creates a histogram of the strip received
 */
class Worker: public ff::ff_node
{

public:
    int svc_init();
    void *svc(void *task);
    void svc_end();
};

int Worker::svc_init()
{
    return 0;
}

void* Worker::svc(void* task)
{
    char* strip = (char*) task;
    int imageSize = strlen(strip);

    for( int currentIndex = 0; currentIndex < imageSize; currentIndex++ )
    {
        unsigned char currentVoxel = strip[currentIndex];
        histogram[currentVoxel]++;
    }
    return (void *) histogram.size();
}

void Worker::svc_end()
{
}


/**
 * @brief The Collector class
 *
 * reduce all the histograms
 * calculates L1 and L2
 */
class Collector : public ff::ff_node
{

public:
    int svc_init();
    void *svc(void *task);
    void svc_end();

};


int Collector::svc_init()
{
    return 0;
}

void* Collector::svc(void *task)
{
    return GO_ON;
}

void Collector::svc_end()
{
    std::map<char,int>::iterator it = histogram.begin();
    for(; it != histogram.end(); it++ ) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << "Done!" << std::endl;
}





static void
bufferImage(char* filename, int size)
{
    imageBuffer = (unsigned char*) malloc(sizeof(unsigned char)*size*size*size);
    imageBuffer = readImage(filename);
}




/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    int nWorkers = 10;
    imageSize = 400;
    char* imageFile = "/home/nvno/fct/CPD/workspace/WA3/images/cubo_400.mat";

    if( argc == 4 )
    {
        std::cerr << "USAGE: histogram #workers image_size image_file ("
                  << argc << ")" << std::endl;

        nWorkers = atoi(argv[1]);
        imageSize = atoi(argv[2]);
        imageFile = argv[3];
    }

    bufferImage(imageFile, imageSize);

    ff::ff_farm<> farm;
    std::vector<ff::ff_node*> workers;
    for(int i=0; i < nWorkers; ++i)
    {
        workers.push_back(new Worker);
    }

    farm.add_emitter(new Emitter());
    farm.add_workers(workers);
    farm.add_collector(new Collector);

    // START PIPELINE !!!
    if( farm.run_and_wait_end() < 0 )
    {
        return -1;
    }

    std::cout << "DONE, time= " << farm.ffTime() << " (ms)\n" << std::endl;
    return 0;
}
