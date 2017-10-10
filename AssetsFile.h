#ifndef ASSETSFILE_H_LJSKLKSLDSLKSLKFLKSDLF
#define ASSETSFILE_H_LJSKLKSLDSLKSLKFLKSDLF
#include "Zip.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class AssetsFile : public ReadFileInt
{
public:
    AssetsFile();
    virtual ~AssetsFile();
    
public:
    virtual bool open(std::string filepath);
    virtual int length();
    virtual int read(void* buffer, int size);
    virtual void seek(int offset, int where);
    virtual void close();
    
private:
    AAsset* m_file;
};
#endif