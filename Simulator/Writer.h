#ifndef WRITER
#define WRITER

/*
 * Writer abstract class
 */
class Writer
{
 public:

  /*
   * Virtual destructor of the Writer class
   */
  virtual ~Writer() { }

  /*
   * Purely virtual method to write buffer
   *
   * @param rBuffer Buffer to write
   * @return Negative number if error occurs, 0 otherwise
   */
  virtual int write( const std::string& rBuffer ) = 0;

};

#endif
