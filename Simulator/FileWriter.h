#ifndef FILEWRITER
#define FILEWRITER

#include <string>
#include <fstream>
#include <iostream>
#include "Writer.h"

class FileWriter : public Writer
{
 public:
  FileWriter( const std::string& rFileName ) { mFileStream.open( rFileName ); }

  virtual ~FileWriter() { mFileStream.close(); }

  virtual int write( const std::string& rBuffer ) { mFileStream << rBuffer << std::endl; }

 private:
  std::ofstream mFileStream;

};

#endif
