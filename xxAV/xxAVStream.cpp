#include "xxAV.h"
#include "xxAVStreamPrivate.h"

xxAVStream::xxAVStream()
{
    imp = new xxAVStreamPrivate();
    imp->codecpar = avcodec_parameters_alloc();
}
xxAVStream::~xxAVStream()
{
    if (imp->codecpar != nullptr)
    {
        avcodec_parameters_free(&imp->codecpar);
        imp->codecpar = nullptr;
    }

     if (imp != nullptr)
    {
        delete imp;
        imp = nullptr;
    }
    
}
