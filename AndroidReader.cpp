#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <sys/types.h>
#include "Common.h"
#include "AssetsFile.h"
#include "AndroidReader.h"
#include "IfcFile.h"
#include "IfcFileStream.h"
#include "IfcStream.h"
#include "IMemoryStream.h"
#include "CEngFile.h"
#include "CEngFileStream.h"
#include "CMemoryFileStream.h"
#include "debugger.h"
using namespace engdebugger;
extern AAssetManager* g_assertMgr;
std::string getAssertPath(const char * filename)
{
	std::string tmp = filename;
	const char * keywords = "asdf/";
	if (tmp.find(keywords) != -1)
	{
		return tmp.substr(tmp.rfind(keywords) + strlen(keywords));
	}
	else
	{
		return tmp;
	}
}
AssetsZipInfo::AssetsZipInfo(std::string filename)
{
	m_zipFile.open(filename);
	zGetFileList(&m_zipFile, m_fileList);
	assetsfilename = filename;
}
int AssetsZipInfo::findFile(const char *filename, ZipFile &file)
{
	std::string strfn;
	std::string tmp = filename;
	if (tmp.find('/') != -1)
		strfn = tmp.substr(tmp.rfind('/') + 1);
	else
		strfn = tmp;

	std::map<std::string, ZipFile>::iterator iterIdx = m_fileList.find(strfn);
	if (iterIdx != m_fileList.end())
	{
		file = iterIdx->second;
		return 1;
	}
	return -1;
}

AssetsZipInfo::~AssetsZipInfo()
{
	m_zipFile.close();
} 

AndroidReader::AndroidReader()
{
	if (!g_assertMgr) {
		DebugOut(CPP_ERROR, "android asset manager is null");
		return;
	}

	AAssetDir* assetdir = AAssetManager_openDir(g_assertMgr, "");
	if (!assetdir) {
		DebugOut(CPP_ERROR, "android asset path is empty");
		return;
	}

	for(;;) {
		const char* lpFileName = AAssetDir_getNextFileName(assetdir);
		if (lpFileName == NULL) {
			DebugOut(CPP_ERROR, "file name is null");
			break;
		}

		if (strstr(lpFileName, ".dat") != NULL)
		{
			DebugOut(CPP_ERROR, "asserts file name is %s",lpFileName);
			AssetsZipInfo* tmp = ENG_NEW AssetsZipInfo(lpFileName);
			if (m_zipfiles[lpFileName] != NULL)
			{
				SAFE_DEL(m_zipfiles[lpFileName]);
			}
			m_zipfiles[lpFileName] = tmp;
		}
    }
    
    AAssetDir_close(assetdir);
}

AndroidReader::~AndroidReader()
{
	for (std::map<std::string, AssetsZipInfo*>::iterator it = m_zipfiles.begin(); it != m_zipfiles.end(); ++it)
	{
		SAFE_DEL(it->second);		
	}
	m_zipfiles.clear();
}
bool AndroidReader::isExist(const char *filename)
{
	ZipFile file;
	if (findFile(filename, file) != -1)
		return true;
	if (g_assertMgr != NULL)
	{
		AAsset* ashandle = AAssetManager_open(g_assertMgr, getAssertPath(filename).c_str(), 0);
		if(ashandle != NULL)
		{				
			AAsset_close(ashandle);
			return true;			
		}
	}
	return false;
}
size_t AndroidReader::getFileSize(const char *filename)
{
	ZipFile file;
	if (findFile(filename, file) != -1)
		return file.fileSize;
	
	if (g_assertMgr != NULL)
	{		
		AAsset* ashandle = AAssetManager_open(g_assertMgr, getAssertPath(filename).c_str(), 0);
		if(ashandle != NULL)
		{	
			int flength = AAsset_getLength(ashandle);			
			AAsset_close(ashandle);
			return flength;			
		}
	}
	return 0;
}
int AndroidReader::findFile(const char *filename, ZipFile &file)
{
	std::string strfn;
	std::string tmp = filename;
	if (tmp.find('/') != -1)
		strfn = tmp.substr(tmp.rfind('/') + 1);
	else
		strfn = tmp;

	for (std::map<std::string, AssetsZipInfo*>::iterator it = m_zipfiles.begin(); it != m_zipfiles.end(); ++it)
	{
		std::map<std::string, ZipFile>::iterator iterIdx = it->second->m_fileList.find(strfn);
		if (iterIdx != it->second->m_fileList.end())
		{
			file = iterIdx->second;
			return 1;
		}
	}
	return -1;
}
IfcFileStreamPtr AndroidReader::openFile(ZipFile &fileInfo)
{
    char *fileData = NULL;
    int memBufSize = 0;

	for (std::map<std::string, AssetsZipInfo*>::iterator it = m_zipfiles.begin(); it != m_zipfiles.end(); ++it)
	{
		ZipFile tmp;
		if (it->second->findFile(fileInfo.fileName.c_str(), tmp) != -1)
		{
			if (zGetFileContent(&it->second->m_zipFile, fileInfo, fileData, memBufSize))
				return IfcFileStreamPtr(ENG_NEW CMemoryFileStream(fileData, memBufSize, fileInfo.fileName.c_str()));
		}
	}
	
	return IfcFileStreamPtr();	
}
IfcFileStreamPtr AndroidReader::openFile(const char *filename)
{
	ZipFile zfile;
	if (findFile(filename, zfile) != -1)
		return openFile(zfile);
	if (g_assertMgr != NULL)
	{
		AAsset* ashandle = AAssetManager_open(g_assertMgr, getAssertPath(filename).c_str(), 0);
		if(ashandle != NULL)
		{	
			int flength = AAsset_getLength(ashandle);
			char * content = new char[flength];
			int result = AAsset_read(ashandle, content, flength);
			AAsset_close(ashandle);
			return IfcFileStreamPtr(ENG_NEW CMemoryFileStream(content, flength, getAssertPath(filename).c_str()));
		}
	}
	return IfcFileStreamPtr();
}




