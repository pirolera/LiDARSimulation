#include "JSONFileWriter.h"

JSONFileWriter::JSONFileWriter( const std::string& rFileName ) :
  mHasData( false )
{ 
  mFileStream.open( rFileName ); 
  mFileStream << "[" << std::endl; 
}

int JSONFileWriter::write( const std::string& rBuffer ) 
{ 
  if ( mHasData )
  {
    mFileStream << "," << std::endl;
  }
  mFileStream << rBuffer;
  mHasData = true;

  return 0;
}
