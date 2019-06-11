#include "leveldb/db.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "table/block.h"
#include <cassert>
#include<stdio.h>

using namespace std;
using namespace leveldb;


int main() {
  //opening a database
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
  assert(status.ok());
  for(int i = 0; i < 100;i++ ){
    std::string key1="key" + i;
    std::string value1="value" + i;
    status=db->Put(leveldb::WriteOptions(),key1,value1);
  }
//  db->Put(WriteOptions(),"k1","v1");
//  db->Put(WriteOptions(),"k2","v2");
//  db->Put(WriteOptions(),"k3","v3");
//  db->Put(WriteOptions(),"k4","v4");
//  db->Put(WriteOptions(),"k5","v5");
//  db->Put(WriteOptions(),"k6","v6");
//  db->Put(WriteOptions(),"k7","v7");
//  db->Put(WriteOptions(),"k8","v8");
//  db->Put(WriteOptions(),"k9","v9");
//  db->Put(WriteOptions(),"k10","v10");

//  db->Put(WriteOptions(),"k11","v1");
//  db->Put(WriteOptions(),"k12","v2");
//  db->Put(WriteOptions(),"k13","v3");
//  db->Put(WriteOptions(),"k14","v4");
//  db->Put(WriteOptions(),"k15","v5");
//  db->Put(WriteOptions(),"k16","v6");
//  db->Put(WriteOptions(),"k17","v7");
//  db->Put(WriteOptions(),"k18","v8");
//  db->Put(WriteOptions(),"k19","v9");
//  db->Put(WriteOptions(),"k20","v10");

//  ifstream in("test.ldb");
//  assert(in.is_open());
//  in.seekg(0,ios::end);       //基地址为文件结束处，偏移地址为0，于是指针定位在文件结束处
//  streampos sp=in.tellg(); //sp为定位指针，因为它在文件结束处，所以也就是文件的大小
//  cout<<"file size:"<<endl<<sp<<endl;
//
//
//
//  in.seekg(-sp/3,ios::end); //基地址为文件末，偏移地址为负，于是向前移动sp/3个字节
//  streampos sp2=in.tellg();
//  cout<<"from file to point:"<<endl<<sp2<<endl;
//
//
//  in.seekg(48,ios::beg);        //基地址为文件头，偏移量为0，于是定位在文件头
//  cout<<in.rdbuf();             //从头读出文件内容
////  in.seekg(sp2);
//  fseek(in,-100L,2);
//
//  cout<<in.rdbuf()<<endl;     //从sp2开始读出文件内容
  delete db;
  return 0;





//  string s;
//  db->Get(ReadOptions(),"k1",&s);
//
//  return 0;
}