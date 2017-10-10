#pragma once

#include "IfcFile.h"
#include <stdio.h>

#include <string>

class CEngFile : public IfcFile
{
public:

	CEngFile(const char* fileName);

	~CEngFile(void);

	IfcStreamPtr openForStreaming(IfcStream::EStreamingMode::id mode);

	int getSize() const
	{
		return m_size;
	}

	bool isExist() const {
		return m_isExist;
	}

private:

	bool	m_isExist;

	int		m_size;
};
