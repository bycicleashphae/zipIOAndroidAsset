#pragma once

#include "IfcFileStream.h"
#include "IfcFile.h"

#include <string>

class CEngFileStream : public IfcFileStream
{
public:
	CEngFileStream(const IfcFilePtr& file, int accessMode);

	CEngFileStream(const char *filename, int accessMode);

	~CEngFileStream(void);

	int	read(void *outBuf, int size) const	;

	int write(const char *data, int size)	;

	bool seek(int offset, int pos_type)	;

	int tell() const						;

	bool isReachEnd() const					;

    bool accessAble() const;
    
    bool openForStreaming();

private:
	FILE	*m_fileHandle;
};
