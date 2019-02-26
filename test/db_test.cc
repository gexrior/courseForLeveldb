#include "leveldb/db.h"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace leveldb;

int main() {
  //opening a database
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
  assert(status.ok());

  std::string key1="book";
  std::string value1="algorithm";
  std::string value;
  db->Put(WriteOptions(),key1,value1);
  db->Get(ReadOptions(),key1,&value);
  std::cout<<"key:"<<key1<<",value:"<<value<<std::endl;


  delete db;

  return 0;
}