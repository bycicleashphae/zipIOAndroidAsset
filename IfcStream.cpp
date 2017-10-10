#include "stdafx.h"
#include "IfcStream.h"

bool IfcStream::readBool() const
{
	char value;
	read((char*)&value, sizeof(char));
	return value != 0;
}

void IfcStream::writeBool(bool value)
{
	char v = value ? 1 : 0;
	write((char*)&v, sizeof(char));
}

int IfcStream::readInt() const
{
	int value;
	read((char*)&value, sizeof(int));
	return value;
}

void IfcStream::writeInt(int value)
{
	write((char*)&value, sizeof(int));
}

short IfcStream::readShort() const
{
	short value;
	read((char*)&value, sizeof(short));
	return value;
}

void IfcStream::writeShort(short value)
{
	write((char*)&value, sizeof(short));
}

char IfcStream::readChar() const
{
	char value;
	read((char*)&value, sizeof(char));
	return value;
}

void IfcStream::writeChar(char value)
{
	write((char*)&value, sizeof(char));
}

int IfcStream::readString(char* outBuf) const
{
	short len = readShort();
	return read(outBuf, len);
}

int IfcStream::writeString(char *str, int len)
{
	writeShort(len);
	return write(str, len);
}

bool IfcStream::accessAble() const
{
    return false;
}
