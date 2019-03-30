#ifndef JSONFILEWRITER
#define JSONFILEWRITER

#include <string>
#include <fstream>
#include <iostream>
#include "Writer.h"

/*
 * Writer class to write buffer in JSON file
 */
class JSONFileWriter : public Writer
{
 public:

  /*
   * Construction for JSONFileWriter class
   *
   * @param rFileName Name of the file to write the buffers in
   */
  JSONFileWriter( const std::string& rFileName );


  /*
   * Destruction for JSONFileWriter class
   */
  virtual ~JSONFileWriter() { mFileStream << std::endl << "]" << std::endl; mFileStream.close(); }


  /*
   * Method to write buffer
   *
   * @param rBuffer Buffer to write
   * @return Negative number if error occurs, 0 otherwise
   */
  virtual int write( const std::string& rBuffer );

 private:

  //File stream where to write the buffer
  std::ofstream mFileStream;

  //Boolean whether data has been written already, used to determine the addition of ","
  bool mHasData;

};

#endif
