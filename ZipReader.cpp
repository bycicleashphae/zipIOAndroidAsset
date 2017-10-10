#include "stdafx.h"
#include "IMemoryStream.h"

#include "CMemoryFileStream.h"
#include "CEngFileStream.h"
#include "ZipReader.h"


void zipFileReader::close()
{
	fclose(m_file);
	m_file = NULL;
}

void zipFileReader::seek(int offset, int where)
{
    fseek(m_file, offset, where);
}


CZipReader::CZipReader(const IfcFileStreamPtr &fs)
{
	m_sFStream = fs;
	scanHeader();
}

CZipReader::~CZipReader()
{
}

int zipFileReader::read(void* buffer, int size)
{
	int readSize = fread(buffer, size, 1, m_file);
	return readSize;
}

size_t CZipReader::getFileSize(const char *filename)
{
	ZipFile file;
	int index = findFile(filename, file);
	if (index != -1)
		return file.fileSize;
	return 0;
}

bool CZipReader::scanHeader()
{
	std::string fn = m_sFStream->getFileName();
	if (!m_zipFile.open(fn))
		return false;

	zGetFileList(&m_zipFile, m_fileList);
    return true;
}

int zipFileReader::length()
{
	fseek(m_file, 0, SEEK_END);
	return ftell(m_file);
}

IfcFileStreamPtr CZipReader::openFile(const char *fn)
{
	ZipFile f;    
	if (findFile(fn, f) == -1)
		return IfcFileStreamPtr();
	return openFile(f);
}


bool CZipReader::isExist(const char *filename)
{
	ZipFile file;
	if (findFile(filename, file) != -1)
		return true;
	return false;
}
IfcFileStreamPtr CZipReader::openFile(ZipFile &fileInfo)
{   
    char *data = NULL;
    int bufsize = 0;
	if (!zGetFileContent(&m_zipFile, fileInfo, data, bufsize))
		return IfcFileStreamPtr();		
	return IfcFileStreamPtr(new CMemoryFileStream(data, bufsize, fileInfo.fileName.c_str()));
}

int CZipReader::findFile(const char *sfn, ZipFile &fileOut)
{
	std::string strtmp = sfn;
	std::string filename;
	if (strtmp.find('/') != -1)
		filename = strtmp.substr(strtmp.rfind('/') + 1);
	else
		filename = strtmp;
	std::map<std::string, ZipFile>::iterator iterIdx = m_fileList.find(filename);
	if (iterIdx == m_fileList.end())
		return -1;
	fileOut = iterIdx->second;
	return 1;
}


bool zipFileReader::open(std::string filepath)
{
	m_file = fopen(filepath.c_str(), "rb");
	return m_file != NULL;
}
