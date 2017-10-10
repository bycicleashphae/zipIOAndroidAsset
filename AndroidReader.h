#ifndef ANDROIDASSERTREADER_FASHJASLLDKFLJLASDFLK
#define ANDROIDASSERTREADER_FASHJASLLDKFLJLASDFLK
#include <vector>
#include "AssetsFile.h"
#include "IfcFileStream.h"
class AssetsZipInfo
{

public:
	AssetsZipInfo(std::string filename);
	~AssetsZipInfo();
	bool open(const char* filemane);
	bool close();
	int findFile(const char *filename, ZipFile &file);
	AssetsFile m_zipFile;
	std::map<std::string, ZipFile> m_fileList;
	std::string assetsfilename;
};

class AndroidReader
{
public:
	AndroidReader();
	virtual ~AndroidReader();
    
public:
	bool isExist(const char *filename);

	size_t getFileSize(const char *filename);

	int findFile(const char *filename, ZipFile &file);

	virtual IfcFileStreamPtr openFile(const char *filename);
    
	IfcFileStreamPtr openFile(ZipFile &fileInfo);
    
    int getFileCount() {
		int cnt = 0;		
		for (std::map<std::string, AssetsZipInfo*>::iterator it = m_zipfiles.begin(); it != m_zipfiles.end(); ++it)
		{
			cnt += it->second->m_fileList.size();
		}
		return cnt;
    }
    


private:
	std::map<std::string, AssetsZipInfo*> m_zipfiles;

};

#endif
