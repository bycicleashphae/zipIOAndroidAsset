#include "stdafx.h"
#include "CEngFile.h"
#include "CEngFileStream.h"
#include <errno.h>

CEngFile::CEngFile(const char* fileName) : IfcFile(fileName)
{

	m_size = -1;
	m_isExist = false;
	std::string strFileName = fileName;
	FILE* file = fopen(strFileName.c_str(), "rb");
	if(file == NULL)
	{
	
		return ;
	}
	fseek(file, 0, SEEK_END);
	m_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	m_isExist = true;
	fclose(file);
}

CEngFile::~CEngFile(void)
{
	m_isExist = false;
	m_size = -1;
}

IfcStreamPtr CEngFile::openForStreaming(IfcStream::EStreamingMode::id mode)
{
	if(!isExist())
	{
		return IfcStreamPtr();
	}

	return IfcStreamPtr(new CEngFileStream(IfcFilePtr(this), (int)mode));
}






