#include "stdafx.h"
#include "CEngFile.h"
#include "IfcFileStream.h"
#include "CEngFileStream.h"

CEngFileStream::CEngFileStream(const char *filename, int accessMode) : IfcFileStream(IfcFilePtr(), accessMode)
{
	m_fileHandle = NULL;
	IfcFilePtr file(new CEngFile(filename));
	if(file->isExist() || getAccessMode() & EStreamingMode::FAM_WRITE)
	{
		attachFile(file);
        openForStreaming();
    }
}

CEngFileStream::CEngFileStream(const IfcFilePtr &file, int accessmode) :IfcFileStream(file, accessmode)
{
	m_fileHandle = NULL;
	if(!file.get() || !file->isExist())
	{
		return ;
	}
    openForStreaming();
}

CEngFileStream::~CEngFileStream(void)
{
	if(m_fileHandle)
	{
		fclose(m_fileHandle);
	}
    
}

bool CEngFileStream::openForStreaming()
{
	std::string strFileName = getFile()->getFileName();

    if(getAccessMode() & IfcStream::EStreamingMode::FAM_WRITE)
    {
		m_fileHandle = fopen(strFileName.c_str(), "wb");
    }
    else
    {
		m_fileHandle = fopen(strFileName.c_str(), "rb");
    }
    
    return accessAble();
}

bool CEngFileStream::accessAble() const
{
	return m_fileHandle != NULL;
}

int CEngFileStream::read(void *outBuf, int size) const
{
	return fread(outBuf, sizeof(char), size, m_fileHandle);

}

int CEngFileStream::write(const char *data, int size)
{
	return fwrite(data, sizeof(char), size, m_fileHandle);

}

bool CEngFileStream::seek(int offset, int pos_type)
{
	if(fseek(m_fileHandle, offset, pos_type) == 0)
	{
		return true;
	}
	return false;
}

int CEngFileStream::tell() const
{
	return ftell(m_fileHandle);
}

bool CEngFileStream::isReachEnd() const
{
	return feof(m_fileHandle);
}

