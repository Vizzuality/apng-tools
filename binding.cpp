
#include <stdlib.h>
#include <iostream>
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

int main(int argc, char **argv)
{
    std::cout << "If this shows up the main compiles and runs!" << std::endl;
}

OptimizerResult optAPNG(long pngBufferPtr, long size, Options options, long callbackPtr)
{
    unsigned int first, loops, coltype;
    std::shared_ptr<APNGOpt> apngOpt = std::make_shared<APNGOpt>(reinterpret_cast<void (*)(float)>(callbackPtr));
    std::vector<APNGFrame> frames;
    OptimizerResult result = {0, 0};

    int res = apngOpt->load_apng(reinterpret_cast<void *>(pngBufferPtr), size, frames, first, loops);
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
