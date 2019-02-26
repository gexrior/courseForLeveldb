//
// Created by rui on 19-2-26.
//

#include "leveldb/db.h"
#include <cstdio>
#include <iostream>
#include <include/leveldb/write_batch.h>

using namespace leveldb;

int main2(){
  //opening a database
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing=true;
  leveldb::Status status=leveldb::DB::Open(options,"mydb",&db);
  assert(status.ok());

  std::string key1="book";
  std::string value1="algorithm";
  status=db->Put(leveldb::WriteOptions(),key1,value1);
  assert(status.ok());

  std::string value;
  status=db->Get(leveldb::ReadOptions(),key1,&value);
  assert(status.ok());
  //std::cout<<"book:"<<value<<std::endl;

  std::string key2="fruit";
  std::string value2="apple";
  if (status.ok()) {
    leveldb::WriteBatch batch;
    batch.Delete(key1);
    batch.Put(key2, value);
    status = db->Write(leveldb::WriteOptions(), &batch);
  }

  leveldb::Iterator *iter=db->NewIterator(leveldb::ReadOptions());
  for(iter->SeekToFirst();iter->Valid();iter->Next())
  {
    std::cout<<iter->key().ToString()<<":"<<iter->value().ToString()<<std::endl;
  }
  delete iter;

  delete db;
  return 0;
}