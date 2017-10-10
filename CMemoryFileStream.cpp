#include "stdafx.h"
#include "CMemoryFileStream.h"

CMemoryFileStream::CMemoryFileStream(const char *data, int size, const char *filename)
	: IfcFileStream(IfcFilePtr(), IfcStream::EStreamingMode::FAM_READ |IfcStream::EStreamingMode::FAM_WRITE)
	, IMemoryStream(data, size, true)
{
	setAccessMode(IfcStream::EStreamingMode::FAM_READ | IfcStream::EStreamingMode::FAM_WRITE);
	m_filename = filename;
}

CMemoryFileStream::~CMemoryFileStream()
{

}

bool CMemoryFileStream::accessAble() const
{
    return IMemoryStream::accessAble();
}

bool CMemoryFileStream::openForStreaming()
{
    return IMemoryStream::openForStreaming();
}