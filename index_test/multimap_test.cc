//
// Created by rui on 19-5-29.
//

#include "leveldb/db.h"
#include <cstdio>
#include <iostream>
#include <include/leveldb/write_batch.h>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;
using namespace leveldb;

string Trim(string& str)
{
  str.erase(0,str.find_first_not_of(" \t\r\n"));
  str.erase(str.find_last_not_of(" \t\r\n") + 1);
  return str;
}


int main(){
  //opening a database
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing=true;
  options.compression = kNoCompression;
  leveldb::Status status=leveldb::DB::Open(options,"dbindex",&db);
  assert(status.ok());

  //input data
  ifstream fin("../index_test/TestData.csv");
  string line;
  while (getline(fin, line))
  {

    istringstream sin(line);
    vector<string> fields;
    string field;
    while (getline(sin, field, ','))
    {
      fields.push_back(field);
    }
    string key = Trim(fields[0]);
    string value = Trim(fields[1]);
    status=db->Put(leveldb::WriteOptions(),key,value);
    assert(status.ok());
  }

  std::multimap<std::string,std::string> mmap;

  // iterator
  leveldb::Iterator *iter=db->NewIterator(leveldb::ReadOptions());
  for(iter->SeekToFirst();iter->Valid();iter->Next())
  {
    mmap.insert(std::make_pair(iter->value().ToString(),iter->key().ToString()));
  }



  delete iter;
  return 0;
}