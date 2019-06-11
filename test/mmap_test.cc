//
// Created by rui on 19-6-4.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(){
  // 读文件
  ifstream inFile("test.csv", ios::in);
  string lineStr;
  vector<vector<string>> strArray;
  while (getline(inFile, lineStr))
  {
    // 打印整行字符串
    cout << lineStr << endl;
    // 存成二维表结构
    stringstream ss(lineStr);
    string str;
    vector<string> lineArray;
    // 按照逗号分隔
    while (getline(ss, str, ','))
      lineArray.push_back(str);
    strArray.push_back(lineArray);
  }
}