#ifndef JSONFILEWRITER
#define JSONFILEWRITER

#include <string>
#include <fstream>
#include <iostream>
#include "Writer.h"

class JSONFileWriter : public Writer
{
 public:
  JSONFileWriter( const std::string& rFileName ) { mFileStream.open( rFileName ); mFileStream << "[" << std::endl; }

  //ToDo, remove last ,
  virtual ~JSONFileWriter() { mFileStream << "]" << std::endl; mFileStream.close(); }

  virtual int write( const std::string& rBuffer ) { mFileStream << rBuffer << "," << std::endl; }

 private:
  std::ofstream mFileStream;

};

#endif
