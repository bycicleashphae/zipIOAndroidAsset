#pragma once

#include "IfcStream.h"
#include "IfcFile.h"

class IfcFileStream : virtual public IfcStream
{
public:

	IfcFileStream(const IfcFilePtr &file,int accessMode);
	virtual ~IfcFileStream();

	virtual IfcFilePtr getFile() const ;

	virtual void attachFile(IfcFilePtr file) ;

	virtual bool isFileExist() const ;

	virtual int getFileSize() const ;

	virtual const char* getFileName() const;
    
private:
	IfcFilePtr m_file;
	mutable int m_fileSize;
};

inline
IfcFileStream::IfcFileStream(const IfcFilePtr &file, int accessMode) : IfcStream(accessMode)
{
	m_file = file;
	m_fileSize = -1;

	setAccessMode(accessMode);
}

inline
IfcFileStream::~IfcFileStream()
{
}

inline
IfcFilePtr IfcFileStream::getFile() const 
{
	return m_file;
}

inline 
const char* IfcFileStream::getFileName() const
{
	return getFile()->getFileName();
}

inline
void IfcFileStream::attachFile(IfcFilePtr file)
{
	m_file = file;
}

inline
bool IfcFileStream::isFileExist() const
{
	return m_file.get() && m_file->isExist();
}

inline
int IfcFileStream::getFileSize() const
{
	if(m_fileSize == -1)
	{
		if(m_file.get() && m_file->isExist())
		{
			m_fileSize = m_file->getSize();
		}	
	}
	return m_fileSize;
}

typedef SmartPtr<IfcFileStream> IfcFileStreamPtr;


