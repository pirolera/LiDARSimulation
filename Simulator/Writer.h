#ifndef WRITER
#define WRITER

class Writer
{
 public:

  virtual ~Writer() { }

  virtual int write( const std::string& rBuffer ) = 0;

};

#endif
