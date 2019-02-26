//
// Created by stars on 2018/12/29.
//

#ifndef VALUE_INDEX_KD_INDEX_H
#define VALUE_INDEX_KD_INDEX_H
#include "kdtree.h"
#include <vector>
#include <string>
#include "leveldb/slice.h"


namespace leveldb{

class ValueIndex {

public:
    ValueIndex();
    ~ValueIndex();
    // insert a coordinate to kd-tree with bounded data
    void Insert(double* coord, void* data);
    void Insert(const Slice& value, void* data);
    // range query by the given coordinate and range, return num-nearest results
    std::vector<std::string*> Range(double* target_coord, double range, int num);
    std::vector<std::string*> Range(const Slice& value, double range, int num);
    // extract coordinate key from input value
    void ExtractKey(const Slice& data, double* coord);


private:
    // a kd-tree structure for storing index
    kdtree* tree_;
};

} // namespace leveldb

#endif //VALUE_INDEX_KD_INDEX_H
