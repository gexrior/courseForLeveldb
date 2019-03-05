//
// Created by rui on 19-2-26.
//
#include "leveldb/db.h"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace leveldb;

int main() {
  //opening a database
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status=leveldb::DB::Open(options,"mydb2",&db);
  assert(status.ok());

  std::string key1="fruit";
  std::string value1="apple";
  status=db->Put(leveldb::WriteOptions(),key1,value1);
  assert(status.ok());
  leveldb::ReadOptions readoptions;
  readoptions.snapshot=db->GetSnapshot();

  std::string value2="orange";
  status=db->Put(leveldb::WriteOptions(),key1,value2);
  assert(status.ok());
  std::string value;
  //status=db->Get(leveldb::ReadOptions(),key1,&value);
  // snapshot
  status=db->Get(readoptions,key1,&value);
  assert(status.ok());
  std::cout<<value<<std::endl;
  db->ReleaseSnapshot(readoptions.snapshot);

  delete db;

  return 0;

}
