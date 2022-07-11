
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "apngopt.hpp"
#include <memory>

struct Options
{
    int deflate_method = 1;
    int iter = 15;
    int min_quality = 0;
    int max_quality = 100;
    bool disabled_quant = false;
};

struct OptimizerResult
{
    long bufferPtr;
    long size;
};

OptimizerResult optAPNG(void *pngBufferPtr, long size, Options options, void (*callbackPtr)(float))
{
    unsigned int first, loops, coltype;
    std::shared_ptr<APNGOpt> apngOpt = std::make_shared<APNGOpt>(callbackPtr);
    std::vector<APNGFrame> frames;
    OptimizerResult result = {0, 0};

    int res = apngOpt->load_apng(pngBufferPtr, size, frames, first, loops);
    if (res >= 0)
    {
        apngOpt->optim_dirty(frames);
        apngOpt->optim_duplicates(frames, first);
        if (!options.disabled_quant)
        {
            apngOpt->optim_image(frames, coltype, options.min_quality, options.max_quality);
        }
        else
        {
            apngOpt->optim_downconvert(frames, coltype);
        }

        long bufferPtr;
        result.size = (long)apngOpt->save_apng(bufferPtr, frames, first, loops, coltype, options.deflate_method, options.iter);
        result.bufferPtr = bufferPtr;
    }

    for (size_t j = 0; j < frames.size(); j++)
    {
        delete[] frames[j].rows;
        delete[] frames[j].p;
    }
    frames.clear();

    return result;
}

void onProgress(float progress)
{
    std::cout << "Progress: " << progress << std::endl;
}

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        std::cerr << "ERROR: Incorrect number of parameters. 1) input path - 2) output path" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream inputFile(argv[1], std::ios::in | std::ios::binary);

// Stop eating new lines in binary mode
    inputFile.unsetf(std::ios::skipws);

// Determine length of data to read
    inputFile.seekg(0, std::ios::end);
    size_t fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<char> inputBytes;
    inputBytes.reserve(fileSize);
    //input.read(inputBytes.data(), fileSize);
    inputBytes.insert(inputBytes.begin(), std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());

    std::cout << "Read " << fileSize << " bytes" << std::endl;

    Options options;
    OptimizerResult result = optAPNG(&inputBytes[0], fileSize, options, onProgress);

    std::cout << "Process complete. Saving to file " << argv[2]<< std::endl;

    //https://stackoverflow.com/questions/11563963/how-to-write-a-large-buffer-into-a-binary-file-in-c-fast
    auto outputFile = std::fstream(argv[2], std::ios::out | std::ios::binary);
    outputFile.write((char *)result.bufferPtr, result.size);
    outputFile.close();

    std::cout << "Save completed. Cleaning up"<< std::endl;

    free((void*)result.bufferPtr);

    std::cout << "Completed "<< std::endl;
}