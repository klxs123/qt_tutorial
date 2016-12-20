#ifndef __utility_h__
#define __utility_h__

#include <fileservice_def.h>
#include <list>
using std::list;

int list_files(const string& dir, list<FileInfo*>& infos);



#endif
