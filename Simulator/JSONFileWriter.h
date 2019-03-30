#ifndef JSONFILEWRITER
#define JSONFILEWRITER

#include <string>
#include <fstream>
#include <iostream>
#include "Writer.h"

class JSONFileWriter : public Writer
{
 public:
  JSONFileWriter( const std::string& rFileName );

  virtual ~JSONFileWriter() { mFileStream << std::endl << "]" << std::endl; mFileStream.close(); }

  virtual int write( const std::string& rBuffer );

 private:
  std::ofstream mFileStream;

  bool mHasData;

};

#endif
