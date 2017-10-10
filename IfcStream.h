#pragma once

#include "SmartPtr.h"

class IfcStream
{
public:
	struct EStreamingMode
	{
        enum id
        {
            FAM_NONE		= 0,
            FAM_READ		= 1,
            FAM_WRITE		= 2,
        };
	};
	struct SeekType
	{
        enum id{
            PT_BEGIN	= 0,
            PT_CURRENT	= 1,
            PT_END		= 2,
        };
	};

	IfcStream();

	IfcStream(int accessMode);
	virtual ~IfcStream();

	int getAccessMode() const	{ return m_accessMode;	}

	virtual int	read(void *outBuf, int size) const	=	0;

	virtual int write(const char *data, int size)	=	0;

	virtual bool seek(int offset, int pos_type)     =	0;

	virtual int tell() const						=	0;

	virtual bool isReachEnd() const					=	0;
    
    virtual bool    accessAble() const              =   0;
    
    virtual bool    openForStreaming()              =   0;

	virtual bool	readBool() const;

	virtual void	writeBool(bool value);

	virtual int		readInt() const;

	virtual void	writeInt(int value);

	virtual short	readShort() const;

	virtual void	writeShort(short value);

	virtual char	readChar() const;

	virtual void	writeChar(char value);

	virtual int		readString(char *outBuf) const;

	virtual int		writeString(char *str, int len);
    

protected:
	void setAccessMode(int newMode);
private:
	int m_accessMode;
};

inline
IfcStream::IfcStream()
{
	m_accessMode = EStreamingMode::FAM_NONE;
}

inline
IfcStream::IfcStream(int accessMode)
{
	m_accessMode = accessMode;
}

inline
IfcStream::~IfcStream()
{
}

inline
void IfcStream::setAccessMode(int newMode)
{
	m_accessMode = newMode;
}

typedef SmartPtr<IfcStream> IfcStreamPtr; 