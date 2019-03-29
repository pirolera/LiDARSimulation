#ifndef FILEWRITER
#define FILEWRITER

#include <string>
#include <fstream>
#include <iostream>
#include "Writer.h"

class FileWriter : public Writer
{
 public:
  FileWriter( const std::string& rFileName ) { mFileStream.open( rFileName ); mFileStream << "[" << std::endl; }

  //ToDo, remove last ,
  virtual ~FileWriter() { mFileStream << "]" << std::endl; mFileStream.close(); }

  virtual int write( const std::string& rBuffer ) { mFileStream << rBuffer << "," << std::endl; }

 private:
  std::ofstream mFileStream;

};

#endif
