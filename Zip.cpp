#include <stdio.h>
#include "Zip.h"

#define SIGNCODE (0x04034b50)

zFHeaderExt *zGetFileHeader(ReadFileInt *f, size_t off)
{
	zFHeaderExt *fHeader = new zFHeaderExt;
    f->seek(off, SEEK_SET);
	f->read(&fHeader->data, sizeof(zFHeader));	
	if (fHeader->data.sign == SIGNCODE || fHeader->data.sign == 0x01010101)
    {
		f->read(fHeader->file_name, fHeader->data.fnlen);
		fHeader->file_name[fHeader->data.fnlen] = 0;
		return fHeader;
    }
	delete fHeader;
	return NULL;
}

int zGetFileList(ReadFileInt *file, std::map<std::string, ZipFile> &lsfile)
{
	unsigned long cDirOffset = 0;
	zCDirExt *cenDir = NULL;
	zArchiveEnd *archiveEnd = zGetArchiveEnd(file);

	for (int i = 0; i < archiveEnd->dirnum; i++)
	{
		size_t tmoffp = archiveEnd->diroff + cDirOffset;
		if ((cenDir = zGetCentralDir(file, tmoffp)) == NULL)
			break;
		cDirOffset += cenDir->size;
		if (cenDir->data.cmethod != 8 &&
			cenDir->data.cmethod != 0)
		{
			printf("support compress format errro !\n");
			delete cenDir;
			continue;
		}

		zFHeaderExt *zfHeader = NULL;
		if ((zfHeader = zGetFileHeader(file, cenDir->data.fheaderoff)) == NULL)
		{
			delete cenDir;
			continue;
		}

		if (zfHeader->data.fnlen == 0 || zfHeader->file_name[zfHeader->data.fnlen - 1] == '/')
		{
			delete cenDir;
			delete zfHeader;
			continue;
		}

		ZipFile zinfo;
		zinfo.flags = cenDir->data.cmethod;
		zinfo.fileOffset = cenDir->data.fheaderoff;
		zinfo.crc32 = zfHeader->data.crc32;
		zinfo.comSize = cenDir->data.csize;
		zinfo.fileName = zfHeader->file_name;
		zinfo.fileSize = cenDir->data.ucsize;
		std::string zFName;
		if (zinfo.fileName.find('/') != -1)
			zFName = zinfo.fileName.substr(zinfo.fileName.rfind('/') + 1);
		else
			zFName = zinfo.fileName;
		if (!zFName.empty())
			lsfile.insert(std::make_pair(zFName, zinfo));

		delete zfHeader;
		delete cenDir;
	}

	delete archiveEnd;
	return 1;
}

char *zGetFileContent(ReadFileInt *f, size_t off, size_t c_s)
{
	zFHeaderExt *fHeader = zGetFileHeader(f, off);
    char *content = new char[c_s];
	int offset = off + sizeof(zFHeader) + fHeader->data.fnlen + fHeader->data.extlen;
	f->seek(offset, SEEK_SET);
	delete fHeader;
	fHeader = NULL;
    f->read(content, c_s);
    return content;
}



bool zGetFileContent(ReadFileInt *file, ZipFile info, char* &ucb, int &size)
{
	char *cb = zGetFileContent(file, info.fileOffset, info.comSize);

	if (info.flags == 0)
	{
		ucb = cb;
	}else
	if (info.flags == 8)
	{
		ucb = new char[info.fileSize];
		if (!zUncompressBuffer(cb, info.comSize, ucb, info.fileSize))
		{
			printf("zUncompressBuffer error\n");
			delete[]ucb;
			ucb = NULL;
			delete[]cb;
			cb = NULL;
			return false;
		}
		delete[]cb;
		cb = NULL;
	}
	

	size = info.fileSize;

	return true;
}

bool zUncompressBuffer(char *source, size_t source_len, char *dest, size_t dest_len)
{
	char *zcont = new char[source_len + 2];
	zcont[0] = 0x78;
	zcont[1] = 0x9c;
	memcpy(zcont + 2, source, source_len);
	int n = uncompress((unsigned char *)dest, (uLongf*)&dest_len, (unsigned char *)zcont, (uLong)(source_len + 2));
	if (n == Z_BUF_ERROR) {
		return false;
	}
	else if (n == Z_MEM_ERROR) {
		return false;
	}
	else if (n == Z_DATA_ERROR) {

	}
	delete[]zcont;
	return true;
}
#define max(x, y)   (x) > (y) ? (x) : (y)
#define SINGCODE2 (0x06054b50)


#define SIGNCODE3 (0x02014b50)
zCDirExt *zGetCentralDir(ReadFileInt *file, size_t offset)
{
	zCDirExt *centralDir = new zCDirExt;
    file->seek(offset, SEEK_SET);
	file->read(centralDir, sizeof(zCDir));
	if (centralDir->data.sign == SIGNCODE3)
    {
		centralDir->size = sizeof(zCDir)
			+ centralDir->data.fnlen
			+ centralDir->data.extlen
			+ centralDir->data.commentlen;
		return centralDir;
    }
	delete centralDir;
	return NULL;
}

bool zVerifyCrc32(unsigned int s_crc32, char *source, size_t len)
{
	unsigned int crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, (unsigned char *)source, len);
	if (crc != s_crc32) {
		return false;
	}
	return true;
}


zArchiveEnd *zGetArchiveEnd(ReadFileInt *f)
{
	int fsize = f->length();

	f->seek(-(int)sizeof(zArchiveEnd), SEEK_END);

	zArchiveEnd *aEnd = new zArchiveEnd;
	f->read(aEnd, sizeof(zArchiveEnd));

	if (aEnd->sign == SINGCODE2 && aEnd->fcomment == 0)
	{
		return aEnd;
	}
	else
	{
		int mCS = fsize - (1 << 16) - sizeof(zArchiveEnd);
		mCS = max(mCS, 0);
		int mCL = fsize - mCS;
		f->seek(mCL, SEEK_SET);
		char* data = new char[mCL];
		int commentlen = f->read(data, mCL);
		int start = 0;
		for (data = data + commentlen; *(int *)data != SINGCODE2 && start > commentlen; data--)
		{
			start++;
		}
		if (start >= 0)
		{
			memcpy(aEnd, data, sizeof(zArchiveEnd));
		}
		delete[]data;
		return aEnd;
	}
}
