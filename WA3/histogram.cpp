#include <iostream>
#include <fstream>
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


static ff_allocator allocator;



/**
 * @brief The Emitter class
 *
 * distribute strips of the image
 */
class Emitter: public ff::ff_node {

private:
    int sideSize;
    int offSet;
    int repetitionsLeft;
    std::ifstream inFile;

    char* getImageStrip(int offset);

public:
    Emitter(int size, std::string filename);

    int svc_init ();
    void *svc(void *);
    void svc_end ();

};

Emitter::Emitter(int size, std::string filename)
    : sideSize(size), offSet(0), repetitionsLeft(size-1)
{
    inFile = ifstream();
    inFile.open( filename.c_str(), ios::in|ios::binary );
    inFile.seekg(0); // set the pointer to the beginning
}

int Emitter::svc_init()
{
    std::cout << "Emitter Started (" << sideSize << ")" << std::endl ;
    return 0;
}

void* Emitter::svc(void *)
{
    std::cout << "new task" << std::endl;

    while(repetitionsLeft >= 0) {
        char *imageStrip = (char *) allocator.malloc(sideSize*sizeof(char));

        int stripStart = repetitionsLeft + sideSize;

        *imageStrip = ntasks;
        ff::ff_node::ff_send_out((void *) imageStrip);
        repetitionsLeft--;
    }
    ff_send_out(EOS);
    return NULL;



    if( task >= sideSize * sideSize )
    {
        return NULL;
    }

    offSet = task + sideSize;
    return (void*) task;
}

char* Emitter::getImageStrip(int offset)
{
    size_t size = 0; // here

    char* oData = 0;

    inFile.seekg(offset, ios::beg);

    oData = new char[ sideSize+1 ];
    inFile.read( oData, size );
    oData[size] = '\0' ; // set '\0'
    cout << " oData size: " << strlen(oData) << "\n";

    //print data
    for ( size_t i = 0; i < strlen(oData); i++ )
    {
        cout << "oData["<<i<<"] " << oData[i];
        cout << "\n";
        cout << oData[i] << " + 'a' = " << ( oData[i] + 'a' );
        cout << "\n\n";

    }
}


void Emitter::svc_end()
{
    std::cout << "Emitter end." << std::endl;
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
    void svc_end();
};

int Worker::svc_init()
{
    std::cout << "Worker " << ff::ff_node::get_my_id() << " init" << std::endl;
    return 0;
}

void* Worker::svc(void* task)
{
    int strip = (int) task;
    std::cout << "> " << strip << std::endl;
//    int imageSize = strlen((char*)strip);

//    for( int currentIndex = 0; currentIndex < imageSize; currentIndex++ )
//    {
//        unsigned char currentVoxel = strip[currentIndex];
//        histogram[currentVoxel]++;
//    }
    return (void *) task;
}

void Worker::svc_end()
{
    std::cout << "Worker end." << std::endl;
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
    int svc_init();
    void *svc(void *task);
    void svc_end();
    void appendMap(std::map<char,int> map);

};


int Collector::svc_init()
{
    std::cout << "Collector init." << std::endl;
    return 0;
}

void* Collector::svc(void *task)
{
//    std::map<char,int> *partial_histogram = (std::map<char,int>*) task;

//    appendMap( *partial_histogram );

    std::cout << "." << std::endl;

    delete task;
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
    std::cout << "Done!" << std::endl;
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
    int imageSize = 100;
    std::string imageFile = "/home/nvno/fct/CPD/workspace/WA3/images/cube_100.mat";

    if( argc == 4 )
    {
        std::cerr << "USAGE: histogram #workers image_size image_file ("
                  << argc << ")" << std::endl;

        nWorkers = atoi(argv[1]);
        imageSize = atoi(argv[2]);
        imageFile = argv[3];
    }

    ff::ff_farm<> farm;
    std::vector<ff::ff_node*> workers;
    for(int i=0; i < nWorkers; ++i)
    {
        workers.push_back(new Worker);
    }

    //imageBuffer = readImage( imageFile );

    farm.add_emitter(new Emitter(imageSize));
    farm.add_workers(workers);
    farm.add_collector(new Collector);

    // START PIPELINE !!!
    if( farm.run_and_wait_end() < 0 )
    {
        return -1;
    }

    return 0;
}
