
#include "stdafx.h"
#include "IMemoryStream.h"
#if defined(OS_LINUX)
    #include "string.h"
#endif

IMemoryStream::IMemoryStream(int memSize) : IfcStream(IfcStream::EStreamingMode::FAM_READ | IfcStream::EStreamingMode::FAM_WRITE)
{
	reAlloc(memSize);
	setAccessMode(IfcStream::EStreamingMode::FAM_READ | IfcStream::EStreamingMode::FAM_WRITE);
}

IMemoryStream::IMemoryStream(const void* dataPtr, int dataSize, bool ownData)
	: IfcStream(IfcStream::EStreamingMode::FAM_READ | IfcStream::EStreamingMode::FAM_WRITE)
{
	m_ownData	=	ownData;
	m_length	=	dataSize;
	m_pos		=	0;
	m_buffer	=	(char*)dataPtr;
}

IMemoryStream::~IMemoryStream()
{
	if(m_ownData)
	{
		delete []m_buffer;
	}
}

void IMemoryStream::reAlloc(int size)
{
	if(m_ownData)
	{
		delete []m_buffer;
	}

	m_length	=	size;
	m_pos		=	0;
	m_ownData	=	true;
	m_buffer = new char[m_length];
	seek(0, IfcStream::SeekType::PT_BEGIN);
}

bool IMemoryStream::ensureBufferEnough(int sizeToAdd)
{
	if (m_pos + sizeToAdd > m_length)
	{
		char* pOldStream = getBuffer();
		int newAllocated = 2 * (m_pos + sizeToAdd);
		m_buffer = new char[newAllocated];
		memcpy(m_buffer, pOldStream, m_length);
		m_length = newAllocated;
		if(m_ownData)
		{
			if (pOldStream != NULL)
			delete []pOldStream;
		}
	}
	return true;
}

int IMemoryStream::write(const char *data, int size)
{
	if(ensureBufferEnough(size))
	{
		memcpy(m_buffer+m_pos, data, size);
		m_pos += size;
		return size;
	}
	return -1;
}

int IMemoryStream::read(void *outBuf, int size) const
{
	int realSize = size > (m_length - m_pos) ? m_length - m_pos : size;
	

	memcpy(outBuf, m_buffer + m_pos, realSize);
	m_pos += realSize;
	return realSize;
}

bool IMemoryStream::seek(int offset, int pos_type)
{
	if(pos_type == IfcStream::SeekType::PT_CURRENT)
	{
		if(m_pos + offset < m_length)
		{
			m_pos += offset;
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(pos_type == IfcStream::SeekType::PT_BEGIN)
	{
		if(offset < m_length)
		{
			m_pos = offset;
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(pos_type == IfcStream::SeekType::PT_END)
	{
		if(offset <= m_length)
		{
			m_pos = m_length - offset;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int IMemoryStream::tell() const
{
	return m_pos;
}

int IMemoryStream::getLength() const
{
	return m_length;
}

bool IMemoryStream::isOwnData() const
{
	return m_ownData;
}

char* IMemoryStream::getBuffer()
{
	return m_buffer;
}

bool IMemoryStream::isValid() const
{
	return m_buffer != NULL;
}

bool IMemoryStream::isReachEnd() const
{
	return m_pos >= m_length;
}

bool IMemoryStream::accessAble() const
{
    return m_buffer;
}

bool IMemoryStream::openForStreaming()
{
    return false;
}


