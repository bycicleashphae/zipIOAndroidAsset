#pragma once
#include "IfcStream.h"

class IMemoryStream : virtual public IfcStream
{
public:
	IMemoryStream(int memSize);
	IMemoryStream(const void* dataPtr, int dataSize, bool ownData);
	~IMemoryStream();

	int		getLength() const;

	char*	getBuffer();

	void	reAlloc(int size);

	bool	ensureBufferEnough(int sizeToAdd);

	int	read(void *outBuf, int size) const	;

	int write(const char *data, int size)	;

	bool seek(int offset, int pos_type)	;

	int tell() const						;

	bool isReachEnd() const					;

	bool isValid() const;

	bool isOwnData() const;
    
    bool accessAble() const;
    
    bool openForStreaming();


private:
	char*	m_buffer;
	bool	m_ownData;
	int		m_length;
	mutable int	m_pos;
};

typedef SmartPtr<IMemoryStream> IMemoryStreamPtr;
