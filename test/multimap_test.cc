//
// Created by rui on 19-5-31.
//
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using  namespace std;

void swap(int a ,int b)
{
  int m ;
  m = a;
  a = b;
  b = m;
}

void perm(vector<int> list,int k, int m )
{
  int i;
  if(k > m)
  {
    for(i = 0 ; i <= m ; i++)
    {
      cout<<list[i]<<" ";

    }
    cout<<endl;

  }
  else
  {
    for(i = k ; i <= m;i++)
    {
      swap(list[k],list[i]);
      perm(list,k+1,m);
      swap(list[k],list[i]);
    }
  }
}


int main(int argc, char **argv) {
  std::multimap<std::string,int> mmap;
  vector<int> val;
  mmap.insert(std::make_pair("XUR@ecnu.cn",1 ));
  mmap.insert(std::make_pair("XME@sjtu.cn",8));
  mmap.insert(std::make_pair("XUR@ecnu.cn",2));
  mmap.insert(std::make_pair("WAM@ecnu.cn",3));
  mmap.insert(std::make_pair("XUR@ecnu.cn",6));
  mmap.insert(std::make_pair("MMM@ecnu.cn",14));
  mmap.insert(std::make_pair("CMA@fudan.cn",7));
  std::multimap<std::string,int>::iterator it;


  // 统计key为"XUR@ecnu.cn"的数目
  std::string strFind = "XUR@ecnu.cn";
  unsigned int uCount = mmap.count(strFind);
  if (uCount == 0)
  {
    std::cout << "Count " << strFind << ":0"<< std::endl;
  }
  else
  {
    std::cout << "Count " << strFind << ":" << uCount << std::endl;
    std::multimap<std::string, int>::iterator it = mmap.find(strFind);
    if (it != mmap.end())
    {
      for (unsigned int i = 0; i < uCount; ++i)
      {
        val.push_back((*it).second);
        it++;
      }
    }
  }

  //排列
  //use perm() to diy personal settings
  for(int i = 0; i < val.size(); i++){
    for(int j = i +1; j <val.size(); j++){
      cout << val[i] << ", " << val[j] << endl;
    }
  }

  return 0;
}