
#ifndef __fileservice_defs_h__
#define __fileservice_defs_h__
#include <string>
#include <map>
#include <utility>
#include "protocol_utils.h"

using std::string;
using std::map;

enum FileType
{
   FT_File = 0,
    FT_Folder,
    FT_Video,
    FT_Audio,
    FT_Txt,
    FT_Doc
};

struct FileInfo
{
    FileType type;    
    uint64_t bytes;//if type is folder, this field value==0
    uint64_t create_date;//seconds from 1970
    string ower;
    string name;
    enum
    {
        FIELDS=5,
    };

    FileInfo():bytes(0)
    {}

    const string& to_data(string& data) const
    {
        uint32_t t = type;
        data.append((const char*)&t, sizeof(t));
        data+=",";
        data.append((const char*)&bytes, sizeof(bytes));
        data+=",";
        data.append((const char*)&create_date, sizeof(uint64_t));
        data+=",";
        data+=ower;
        data+=",";
        data+=name;
        data+=",";
    }

    FileInfo& from_data(const string& data)
    {
        list<string> info_datas;
        split_string(data, ",", info_datas);
        if(info_datas.size() != FIELDS)
        {
            return *this;
        }

        list<string>::const_iterator cit = info_datas.begin();

        type = (FileType)*(uint32_t*)(cit->data());
        cit++;
        bytes = *(uint64_t*)(cit->data());
        cit++;
        create_date = *(uint64_t*)(cit->data());
        cit++;
        ower = *cit;
        cit++;
        name = *cit;

        return *this;
    }

};

//data: "user name |path"
struct ListCommandRequest
{
    string user;
    string dir;
    const string to_data() const
    {
        string data;

        data+= user;
        data+="|";
        data +=dir;

        return data;
    }
    const string& to_data(string& data) const
    {
        data+= user;
        data+="|";
        data +=dir;

        return data;
    }
    ListCommandRequest& from_data(const string& data)
    {
        int dot_index = data.find("|");
        user = data.substr(0, dot_index);
        dir =  data.substr(dot_index+1, data.length() - dot_index -1);

        return *this;
    }
};

//
struct ListCommandResponse
{    
    list< FileInfo*> infos;
    ~ListCommandResponse()
    {
        for(list< FileInfo*>::iterator it = infos.begin(); it!= infos.end(); it++)
        {
            delete *it;
        }
    }

    const string to_data() const
    {
        string data;
        to_data(data);
        return data;
    }

    const string& to_data(string& data) const
    {
        for(list< FileInfo*>::const_iterator it = infos.begin(); it!= infos.end(); it++)
        {
            (*it)->to_data(data);
            data+="|";
        }

        return data;
    }

    ListCommandResponse& from_data(const string& data)
    {        
        list<string> info_datas;
        split_string(data, "|", info_datas);
        for(list<string>::const_iterator it = info_datas.begin(); it!= info_datas.end(); it++)
        {
            FileInfo* pinfo = new FileInfo;
            pinfo->from_data(*it);
            infos.push_back(pinfo);
        }

        return *this;
    }
};

#endif
