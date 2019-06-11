#include <cstdio>
#include <iostream>
#if HAVE_SNAPPY
#include <snappy.h>
#endif  // HAVE_SNAPPY

inline bool Snappy_GetUncompressedLength(const char* input, size_t length,
                                         size_t* result) {
#if HAVE_SNAPPY
  return snappy::GetUncompressedLength(input, length, result);
#else
  return false;
#endif  // HAVE_SNAPPY
}

inline bool Snappy_Uncompress(const char* input, size_t length, char* output) {
#if HAVE_SNAPPY
  return snappy::RawUncompress(input, length, output);
#else
  return false;
#endif  // HAVE_SNAPPY
}

using namespace std;

int getFileSize(const string &filename) {
  int size = 0;
  FILE *fp = nullptr;

  fp = fopen(filename.c_str(), "r");
  if (nullptr == fp) {
    return size;
  }
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fclose(fp);

  return size;

}

char *GetVarint64Ptr(char *p, const char *limit, uint64_t *value) {
  uint64_t result = 0;
  for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
    uint64_t byte = *(reinterpret_cast<const unsigned char *>(p));
    p++;
    if (byte & 128) {
      // More bytes are present,| or
      result |= ((byte & 127) << shift);
    } else {
      result |= (byte << shift);
      *value = result;
      return p;
    }
  }
  return nullptr;
}

char *GetVarint64(char *input, int size, uint64_t *value) {
  char *limit = input + size;
  char *q = GetVarint64Ptr(input, limit, value);
  return q;
}

struct BlockContents {
  string data;           // Actual contents of data
  bool cachable;        // True iff data can be cached
  bool heap_allocated;  // True iff caller should delete[] data.data()
};

enum CompressionType {
  // NOTE: do not change the values of existing entries, as these are
  // part of the persistent format on disk.
  kNoCompression     = 0x0,
  kSnappyCompression = 0x1
};



int main() {
  string fileName = "test.ldb";
  getFileSize(fileName);
  //文件总大小
  cout << "File Size:" << getFileSize(fileName) << endl;

  FILE *fp = fopen(fileName.c_str(), "r");
  fseek(fp, -48L, SEEK_END);
  char *footer = (char *) malloc(48 * sizeof(char));
  fgets(footer, 48, fp);
  uint64_t metaOffset, metaSize, indexOffset, indexSize;
  footer = GetVarint64(footer, 48, &metaOffset);
  footer = GetVarint64(footer, 38, &metaSize);
  footer = GetVarint64(footer, 28, &indexOffset);
  footer = GetVarint64(footer, 18, &indexSize);
  cout << "Meta offset:" << metaOffset << endl
       << "Meta size:" << metaSize << endl
       << "Index offset:" << indexOffset << endl
       << "Index size:" << indexSize << endl;

  char *index = (char *) malloc((indexSize + 1) * sizeof(char));
  fseek(fp, 0L, static_cast<int>(indexOffset));
  fgets(index, static_cast<int>(indexSize + 1), fp);
  //cout << index[indexSize] << endl;

  //read the block
  BlockContents* result = new BlockContents();
  result->data = string();
  result->cachable = false;
  result->heap_allocated = false;
  //char * handle;
  // Read the block contents as well as the type/crc footer.
  // See table_builder.cc for the code that built this structure.
  size_t n = indexSize;
  switch (index[n]) {
    case kNoCompression:
        // File implementation gave us pointer to some other data.
        // Use it directly under the assumption that it will be live
        // while the file is open.
        result->data = string(index, n);
        result->heap_allocated = false;
        result->cachable = false;  // Do not double-cache
        cout << result->data << endl;
      // Ok
      break;
    case kSnappyCompression: {
      size_t ulength = 0;
      if (!Snappy_GetUncompressedLength(index, n, &ulength)) {
        cout << "corrupted compressed block contents" << endl;
      }
      char* ubuf = new char[ulength];
      if (!Snappy_Uncompress(index, n, ubuf)) {
        delete[] ubuf;
        cout << "corrupted compressed block contents" << endl;
      }
      result->data = string(ubuf, ulength);
      result->heap_allocated = true;
      result->cachable = true;
      cout << result->data << endl;
      break;
    }
    default:
      cout << "bad block type" << endl;
  }

  return 0;
}
