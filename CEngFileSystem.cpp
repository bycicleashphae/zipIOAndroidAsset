#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "stdafx.h"
#include "CEngFileSystem.h"
#include "CEngFile.h"
#include "CEngFileStream.h"
#include "CMemoryFileStream.h"
#include <stdio.h>


 
#ifdef _WIN32
#define snprintf _snprintf
#endif
extern "C" void AddZipFileToFileSystem(const char* pathname)
{
	GET_FS()->addZipFile(pathname);
}
CEngFileSystem::CEngFileSystem(void)
{
}


CEngFileSystem::~CEngFileSystem(void)
{
	for (std::map<std::string, CZipReader*>::iterator iter = m_zipReaders.begin(); iter != m_zipReaders.end(); ++iter)
	{
		if (iter->second != NULL)
			delete iter->second;
	}
	m_zipReaders.clear();
	clear();
}

CEngFileSystem* CEngFileSystem::GetInstance()
{
	static CEngFileSystem fs;
	return &fs;
}

std::string CEngFileSystem::getFileBasename(const std::string& filename, bool keepExtension)
{
 
	int lastSlash = filename.rfind('/');
	const int lastBackSlash = filename.rfind('\\');
	lastSlash = lastSlash>lastBackSlash ? lastSlash : lastBackSlash;
	int end = 0;
	if(!keepExtension)
	{
		end = filename.rfind('.');
		if(end == -1)
		{
			end = 0;
		}
		else
		{
			end = filename.size() - end;
		}
	}


	if(lastSlash < filename.length())
	{
		return filename.substr(lastSlash + 1, filename.size() - lastSlash - 1 - end);
	}
	else if(end != 0)
	{
		return filename.substr(0, filename.size() - end);
	}
	return filename;
}

void CEngFileSystem::clear()
{

}

IfcFileStreamPtr CEngFileSystem::createAndOpenFile(const char* path, const char *mode, bool absolutionpath)
{
 
	int accessMode = IfcStream::EStreamingMode::FAM_NONE;
	bool requreRead = strstr(mode, "r") != 0;
	bool requreWrite = strstr(mode, "w") != 0;
	if(requreRead && requreWrite)
	{
		accessMode = IfcStream::EStreamingMode::FAM_READ | IfcStream::EStreamingMode::FAM_WRITE;
	}
	else if(requreRead)
	{
		accessMode = IfcStream::EStreamingMode::FAM_READ;
	}
	else if(requreWrite)
	{
		accessMode = IfcStream::EStreamingMode::FAM_WRITE;
	}	
	if (absolutionpath) // do not update file path and not found in zip for debug
	{
		CEngFileStream *pFileStream = new CEngFileStream(path, accessMode);
		if (pFileStream->isFileExist()){
			int size = pFileStream->getFileSize();
			char *data = new char[size];
			pFileStream->read(data, size);
			CMemoryFileStream *mfs = new CMemoryFileStream(data, size, path);
			delete pFileStream;
			return IfcFileStreamPtr(mfs);
		}
		else
			return IfcFileStreamPtr();
	}
    char filenameUpdated[512];
	strcpy(filenameUpdated, path);
 
	for (std::map<std::string, CZipReader*>::iterator iterIdx = m_zipReaders.begin(); iterIdx != m_zipReaders.end(); ++iterIdx)
	{
		IfcFileStreamPtr file = iterIdx->second->openFile(path);
		if (file.get())
		{
			return file;
		}
	}
#ifdef OS_ANDROID
	IfcFileStreamPtr file = m_androidReader.openFile(path);
	if (file.get())
	{
		return file;
	}
#endif
	CEngFileStream *pFileStream = new CEngFileStream(filenameUpdated, accessMode);
	return IfcFileStreamPtr(pFileStream);
 
}

int CEngFileSystem::getFileSize(const char* path)
{
    char filenameUpdated[512];
	strcpy(filenameUpdated, path);
	CEngFile file(filenameUpdated);
	if (file.isExist()) return file.getSize();
	for (std::map<std::string, CZipReader*>::iterator iterIdx = m_zipReaders.begin(); iterIdx != m_zipReaders.end(); ++iterIdx)
	{
		size_t size = iterIdx->second->getFileSize(path);
		if (size) return size;
	}
#ifdef OS_ANDROID
	size_t size = m_androidReader.getFileSize(path);
	if (size) return size;
#endif
	return 0;
}

bool CEngFileSystem::isFileExist(const char *path)
{
	char filenameUpdated[512];
	strcpy(filenameUpdated, path);
	CEngFile file(filenameUpdated);

	if (file.isExist()) return true;

	for (std::map<std::string, CZipReader*>::iterator iterIdx = m_zipReaders.begin(); iterIdx != m_zipReaders.end(); ++iterIdx)
	{
		if (iterIdx->second->isExist(path)) return true;
	}

#ifdef OS_ANDROID
	if (m_androidReader.isExist(path)) 
		return true;
#endif
	return false;
 
}
 

void CEngFileSystem::addZipFile(const char *filename)
{
	char filenameUpdated[512];
	strcpy(filenameUpdated, filename);

	CEngFileStream *pFileStream = new CEngFileStream(filenameUpdated, IfcStream::EStreamingMode::FAM_READ);
	IfcFileStreamPtr file = IfcFileStreamPtr(pFileStream);
	if (!file->accessAble())
	{	
		return;
	}
	removeZipFile(filename);
	CZipReader * zipReader = new CZipReader(file);
	m_zipReaders[filename] = zipReader;
}


void CEngFileSystem::removeZipFile(const char *filename)
{
	std::map<std::string, CZipReader*>::iterator iter = m_zipReaders.find(filename);
	if (iter != m_zipReaders.end())
	{
		if (iter->second == NULL)
			delete iter->second;
		m_zipReaders.erase(iter);
	}
}
bool CEngFileSystem::createDirection(const char* path)
{
	int i, len;
	len = strlen(path);
	char buf[512];
	memcpy(buf, path, len + 1);

	for (i = 0; i < len; i++)
	{
		if (buf[i] == '\\' || buf[i] == '\/')
		{
			buf[i] = '\0';
#ifdef _WIN32
			_mkdir(buf);
			buf[i] = '\\';
#else
            mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			buf[i] = '\/';
#endif

		}
	}
    
	if (len > 0)
	{
#ifdef _WIN32
		int err = _mkdir(buf);
#else
		int err = mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
		if (err == -1)
		{
			return false;
		}
	}
	return true;
}

bool CEngFileSystem::writeToFile(const char* data, int len, const char* filepath)
{
	IfcFileStreamPtr fileStream = createAndOpenFile(filepath, "rw");
	if (!fileStream->accessAble())
	{
		return false;
	}
	fileStream->write(data, len);
	return true;
}
 

