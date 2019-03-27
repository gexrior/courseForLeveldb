#include <cstdio>
#include <iostream>

using namespace std;

int getFileSize(const string &filename) {
    int size = 0;
    FILE* fp = nullptr;

    fp = fopen(filename.c_str(), "r");
    if (nullptr == fp) {
        return size;
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return size;

}

char* GetVarint64Ptr(char* p, const char* limit, uint64_t* value) {
    uint64_t result = 0;
    for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
        uint64_t byte = *(reinterpret_cast<const unsigned char*>(p));
        p++;
        if (byte & 128) {
            // More bytes are present
            result |= ((byte & 127) << shift);
        } else {
            result |= (byte << shift);
            *value = result;
            return p;
        }
    }
    return nullptr;
}

char* GetVarint64(char* input, int size, uint64_t* value) {
    char* limit = input + size;
    char* q = GetVarint64Ptr(input, limit, value);
    return q;
}

int main() {
    string fileName = "test.ldb";
    int fileSize = getFileSize(fileName);
    //文件总大小
    cout<<"File Size:"<<getFileSize(fileName)<<endl;

    FILE* fp = fopen(fileName.c_str(), "r");
    fseek(fp, -48L, SEEK_END);
    char* footer = (char *)malloc(48 * sizeof(char));
    fgets(footer, 48, fp);
    uint64_t metaOffset, metaSize, indexOffset, indexSize;
    footer = GetVarint64(footer, 48, &metaOffset);
    footer = GetVarint64(footer, 38, &metaSize);
    footer = GetVarint64(footer, 28, &indexOffset);
    footer = GetVarint64(footer, 18, &indexSize);
    cout<<"Meta offset:"<<metaOffset<<endl
        <<"Meta size:"<<metaSize<<endl
        <<"Index offset:"<<indexOffset<<endl
        <<"Index size:"<<indexSize<<endl;

    char* index = (char *) malloc((indexSize + 1) * sizeof(char));
    fseek(fp, 0L, static_cast<int>(indexOffset));
    fgets(index, static_cast<int>(indexSize + 1), fp);
    //cout<<index[indexSize]<<endl;
    return 0;
}
