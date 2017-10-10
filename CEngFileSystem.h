#pragma once
#include "IfcFileStream.h"

#include <string>
#include <map>
#ifdef OS_ANDROID
#include "AndroidReader.h"
#endif
#include "ZipReader.h"



class CEngFileSystem
{
public:
	CEngFileSystem(void);
	~CEngFileSystem(void);

	static CEngFileSystem* GetInstance();
	IfcFileStreamPtr createAndOpenFile(const char *path, const char *mode = "r",bool absolutionpath=false);

	void clear();

	bool createDirection(const char* path);

	bool writeToFile(const char* data, int len, const char* filepath);

	int getFileSize(const char *path);

	bool isFileExist(const char *path);

	void addZipFile(const char *filename);
	void removeZipFile(const char *filename);

	std::string getFileBasename(const std::string& filename, bool keepExtension);


private:
	std::map<std::string, CZipReader*>     m_zipReaders;
#ifdef OS_ANDROID
	AndroidReader                                m_androidReader;
#endif
};

#define GET_FS() CEngFileSystem::GetInstance()



