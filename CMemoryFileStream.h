#pragma once
#include "IfcFileStream.h"
#include "IMemoryStream.h"
#include <string>
class CMemoryFileStream : public IfcFileStream, public IMemoryStream
{
public:
	CMemoryFileStream(const char *data, int size, const char *filename);

	~CMemoryFileStream(void);

	const char* getFileName() const{
		return m_filename.c_str();
	}

	IfcFilePtr getFile() const{
		return IfcFilePtr();
	}

	void attachFile(IfcFilePtr file){
		return;
	}

	bool isValid() const {
		return IMemoryStream::isValid() && m_filename.length() > 0;
	}

	int getFileSize() const {
		return IMemoryStream::getLength();
	}

	bool isFileExist() const {
		return isValid();
	}
    
    bool accessAble() const;
    
    bool openForStreaming();
private:
	std::string m_filename;
};
