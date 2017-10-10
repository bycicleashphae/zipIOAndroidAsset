
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <sys/types.h>
#include "AssetsFile.h"
#include "debugger.h"
using namespace engdebugger;
AAssetManager* g_assertMgr = NULL;
extern "C"
{
	char assertpathkey[512];
    JNIEXPORT void JNICALL Java_com_test_test_Utils_setAssetManager(JNIEnv * env,jclass cls,jobject assetManager)
    {    
        g_assertMgr = AAssetManager_fromJava(env, assetManager);
    }
};
 

AssetsFile::AssetsFile()
{
    m_file = NULL;
}

AssetsFile::~AssetsFile()
{
    close();
}

bool AssetsFile::open(std::string filepath)
{
	if (!g_assertMgr)
    {
		 
        return false;
    }
	m_file = AAssetManager_open(g_assertMgr, filepath.c_str(), 0);
    return true;
}

int AssetsFile::length()
{
    int fileLength = 0;
    fileLength = AAsset_getLength(m_file);
    return fileLength;
}

int AssetsFile::read(void* buffer, int size)
{
    int readSize = 0;
    readSize = AAsset_read(m_file, buffer, size);
	 
    return readSize;
}

void AssetsFile::seek(int offset, int where)
{
	if (AAsset_seek(m_file, offset, where) == -1)){}
}

void AssetsFile::close()
{
    if (!m_file)
        return;
    
    AAsset_close(m_file);
    m_file = NULL;
}
