
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "apngopt.hpp"

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

OptimizerResult optAPNG(std::shared_ptr<void> pngBufferPtr, long size, Options options, void (*callbackPtr)(float))
{
    unsigned int first, loops, coltype;
    std::shared_ptr<APNGOpt> apngOpt = std::make_shared<APNGOpt>(callbackPtr);
    std::vector<APNGFrame> frames;
    OptimizerResult result = {0, 0};

    int res = apngOpt->load_apng(pngBufferPtr.get(), size, frames, first, loops);
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
    if (argc <= 1)
    {
        std::cerr << "ERROR: No input file provided" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream input(argv[1]);
    input.seekg(0, std::ios::end);
    size_t fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    std::vector<char> inputBytes(fileSize);
    input.read(inputBytes.data(), fileSize);

    // std::shared_ptr<void> apngPointer = std::shared_ptr<void>(&inputBytes[0]);

    std::cout << "Read " << fileSize << " bytes" << std::endl;

    // Options options;
    // optAPNG(apngPointer, fileSize, options, onProgress);

    // std::cout << "If this shows up the main compiles and runs!" << std::endl;
}