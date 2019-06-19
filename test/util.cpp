//
// Created by rui on 19-6-15.
//

#include <cassert>
#include <dirent.h>
#include <format.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "leveldb/iterator.h"
#include "leveldb/slice.h"

#include "db/filename.h"
#include "db/version_set.h"
#include "db/version_edit.h"
#include "db/dbformat.h"



// TODO: 删除 db 文件夹，暂时有点问题
//int DeleteDB(const std::string &db_name){
//    rmdir(db_name.data());
//}

void TEST_ScanTable(){
    leveldb::DB *db;
    leveldb::Options options;
    leveldb::Status status = MyOpen(db_name, &db, options);
    assert(status.ok());

    leveldb::Table* table = nullptr;
    status = OpenFirstTable(db_name, db, options, &table);
    assert(status.ok());
    leveldb::Iterator *table_iter = table->NewIterator(leveldb::ReadOptions());
    ScanTable(table_iter);

    delete db;
}

// 需要指定 bloomfilter, 且数据库需要重新建立。
// table/table_builder.cc 中 add() 函数做了相应修改
void TEST_Filter(){
    DeleteFile(db_name.data());

    leveldb::DB *db;
    leveldb::Options options;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10); // 指定 bloomfilter
    leveldb::Status status = MyOpen(db_name, &db, options);
    assert(status.ok());

    std::cout << "filter name : " << options.filter_policy->Name() << std::endl;

// 打开第一个 table
    leveldb::Table *table = nullptr;
    OpenFirstTable(db_name, db, options, &table);

// 获取第一个 table 的第一个 key,value
// user_key:0, user_value:100000
    std::string key;
    std::string value;
    GetFirstEntry(table, &key, &value);
    bool flag = ValueInTable(table, value);
    std::cout << (true == flag ? "TEST_Filter success" : "TEST_Filter failed")<<std::endl;
}

// 测试每个 table 中最大最小 key
void TEST_ReadSmallAndLargerest(){
    leveldb::DB *db;
    leveldb::Options options;
    leveldb::Status status = MyOpen(db_name, &db, options);
    assert(status.ok());

    leveldb::Version *current = db->GetCurrentVersion();
    for(int i = 0; i< 7;i++){
        std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(i);
        if (files_.size() > 0) {
            for (int i = 0; i < files_.size(); i++) {
                std::cout << files_[i]->number << " ";
                std::cout << files_[i]->smallest.user_key().ToString() << " ";
                std::cout << files_[i]->largest.user_key().ToString() << std::endl;
            }
        }
    }
}

// 输出所有层所有文件的文件号
void PrineAllLevelFile(leveldb::DB *db) {
//    std::cout << "In PrineAllLevelFile." << std::endl;
    leveldb::Version *current = db->GetCurrentVersion();
    for (int i = 0; i < leveldb::config::kNumLevels; i++) {
        std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(i);
        int num = files_.size();
        std::cout << "level " << i << ": ";
        while (num > 0) {
            std::cout << files_[files_.size() - num]->number;
            num--;
        }
        std::cout << std::endl;
    }
}

// 返回第 0 层，第 0 个文件的号
uint64_t GetFirstFileNUmber(leveldb::DB *db) {
    leveldb::Version *current = db->GetCurrentVersion();
    return current->GetFiles(0)[0]->number;
}

// 给定文件号，返回 FileMetaData*
leveldb::FileMetaData *GetTargetFile(uint64_t file_number, leveldb::DB *db) {
//    std::cout << "In GetTargetFile. " << std::endl;
    leveldb::Version *current = db->GetCurrentVersion();
    for (int i = 0; i < leveldb::config::kNumLevels; i++) {
        std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(i);
        for (leveldb::FileMetaData *file : files_) {
            if (file_number == file->number) {
                return file;
            }
        }
    }
}

// 打开第 0 层，第 0 个文件的table
leveldb::Status
OpenFirstTable(std::string db_name, leveldb::DB *db, const leveldb::Options &options, leveldb::Table **table) {
    leveldb::Status status;
    leveldb::FileMetaData *target_file = GetTargetFile(GetFirstFileNUmber(db), db);
    leveldb::RandomAccessFile *file = nullptr;
    status = options.env->NewRandomAccessFile(leveldb::TableFileName(db_name, target_file->number), &file);
    status = leveldb::Table::Open(options, file, target_file->file_size, table);

    return status;
}

// 往db中写入 key(0-100000), value(100000-199999)
leveldb::Status BuildTable_0_100000(leveldb::DB *db) {

    leveldb::Status status;
    for (int i = 0; i < 100000; i++) {
        status = db->Put(leveldb::WriteOptions(), std::to_string(i), std::to_string(i + 100000));
        assert(status.ok());
    }
    std::cout << "Put entries 0-100000 successful." << std::endl;
    return status;
}

// 打开数据库
leveldb::Status MyOpen(std::string db_name, leveldb::DB **db, leveldb::Options &options) {
    bool new_db = false;

    options.comparator = leveldb::NumberComparator();
    leveldb::Status status = leveldb::DB::Open(options, db_name, db);
    if (!status.ok()) {
        new_db = true;
        delete (*db);
        options.create_if_missing = true;
        status = leveldb::DB::Open(options, db_name, db);
        assert(status.ok());
        status = BuildTable_0_100000(*db);
    }

    if (true == new_db) {
        delete (*db);
        status = leveldb::DB::Open(options, db_name, db);
    }

    if (status.ok() && (*db) != nullptr) {
        std::cout << "MyOpen db successful." << std::endl;
    }

    return status;
}

// 扫描某个 table 表
void ScanTable(leveldb::Iterator *table_iter) {
    table_iter->SeekToFirst();
    while (table_iter->Valid()) {
        leveldb::ParsedInternalKey ikey;
        leveldb::ParseInternalKey(table_iter->key(), &ikey);
        std::cout << "key:" << ikey.user_key.ToString();
        std::cout << "   value:" << table_iter->value().ToString() << std::endl;
        table_iter->Next();
    }
}

// 返回某个 value 是否在 某个table 中
bool ValueInTable( leveldb::Table* table, std::string value) {
    uint64_t seq = 2;
    std::string tmp_value = value;
    leveldb::PutFixed64(&tmp_value, seq);
    std::cout << table->ValueInTable(leveldb::ReadOptions(), tmp_value);
}

// 返回某个 table 的第一个entry
void GetFirstEntry(leveldb::Table* table, std::string* key,std::string* value){
    leveldb::Iterator *table_iter = table->NewIterator(leveldb::ReadOptions());
    table_iter->SeekToFirst();
    if (table_iter->Valid()) {
        leveldb::ParsedInternalKey ikey;
        leveldb::ParseInternalKey(table_iter->key(), &ikey);
        *key = ikey.user_key.ToString();
        *value = table_iter->value().ToString();
//        std::cout << std::endl << "first table's first entry:" << std::endl << "user_key:" << user_key << ", user_value:" << user_value << std::endl << std::endl;
    }
}

void Getfilepath(const char *path, const char *filename,  char *filepath)
{
    strcpy(filepath, path);
    if(filepath[strlen(path) - 1] != '/')
        strcat(filepath, "/");
    strcat(filepath, filename);
//    printf("path is = %s\n",filepath);
}

bool DeleteFile(const char* path)
{
    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;
    char filepath[256] = {0};
    lstat(path, &statbuf);

    if (S_ISREG(statbuf.st_mode))//判断是否是常规文件
    {
        remove(path);
    }
    else if (S_ISDIR(statbuf.st_mode))//判断是否是目录
    {
        if ((dir = opendir(path)) == NULL)
            return 1;
        while ((dirinfo = readdir(dir)) != NULL)
        {
            Getfilepath(path, dirinfo->d_name, filepath);
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)//判断是否是特殊目录
                continue;
            DeleteFile(filepath);
            rmdir(filepath);
        }
        closedir(dir);
    }
    return 0;
}