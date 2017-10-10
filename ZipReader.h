#pragma once
#include "Zip.h"
#include "CEngFileStream.h"


class zipFileReader : public ReadFileInt
{
public:
	zipFileReader(){ m_file = NULL; }
	virtual ~zipFileReader(){ close(); }
public:
	virtual void close();
	virtual void seek(int offset, int where);
	virtual int read(void* buffer, int size);
	virtual int length();
	virtual bool open(std::string filepath);
private:
    FILE* m_file;
};
 
class CZipReader
{
public:
	CZipReader(const IfcFileStreamPtr &sFS);

	virtual ~CZipReader();
	bool isExist(const char *fn);
	IfcFileStreamPtr openFile(ZipFile &f);
	size_t getFileSize(const char *fn);
	virtual IfcFileStreamPtr openFile(const char *fn);	
	int findFile(const char *fn, ZipFile &f);
	
 private:
    bool scanHeader();
    IfcFileStreamPtr			m_sFStream;
    std::map<std::string, ZipFile> m_fileList;
	zipFileReader                 m_zipFile;	
};
