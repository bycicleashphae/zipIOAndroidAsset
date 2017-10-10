#pragma once
#include "IfcStream.h"
#include <string>

class IfcFile
{
public:
	IfcFile(const char* filename);
	virtual ~IfcFile();

	virtual bool isExist()	const	=	0;

	virtual int	getSize()	const	=	0;

	virtual IfcStreamPtr openForStreaming(IfcStream::EStreamingMode::id mode)	= 0;

	const char* getFileName() const {
		return m_filename.c_str();
	}
private:
    std::string m_filename;
};

inline
IfcFile::IfcFile(const char* filename):
	m_filename(filename)
{
	
}

inline
IfcFile::~IfcFile()
{

};

typedef SmartPtr<IfcFile> IfcFilePtr;



