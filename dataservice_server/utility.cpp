#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "utility.h"

int list_files(const std::string &path, list<FileInfo *>& infos)
{
    DIR *dir;

    if (!(dir = opendir(path.c_str())))
        return -1;
    struct dirent *entry;
    if (!(entry = readdir(dir)))
    {
        closedir(dir);
        return -1;
    }

    struct stat file_stat;

    do
    {        

        string file_path = path;
        file_path+="/";
        file_path+= entry->d_name;

        if(stat(file_path.c_str(), &file_stat) !=0)
        {
            continue;
        }

        FileInfo* pinfo = new FileInfo;
        pinfo->name = entry->d_name;

        if (entry->d_type == DT_DIR)
        {
            pinfo->type = FT_Folder;
        }
        else
        {
            pinfo->type = FT_File;          

            pinfo->bytes = file_stat.st_size;

        }

        pinfo->create_date = file_stat.st_mtim.tv_sec;
        pinfo->ower = "just for test";


        infos.push_back(pinfo);

    } while (entry = readdir(dir));
    closedir(dir);
    return 0;
}
