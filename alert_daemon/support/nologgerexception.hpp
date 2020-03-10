#ifndef NOLOGGEREXCEPTION_HPP
#define NOLOGGEREXCEPTION_HPP

#include <QString>
#include <exception>

namespace bose_commserver
{
  class NoLoggerException : public std::exception
  {
    protected:
    std::string _msg;

    public:
    explicit NoLoggerException( const char *what ) : _msg( what ){};
    explicit NoLoggerException( const std::string &what ) : _msg( what ){};
    virtual ~NoLoggerException() throw(){};
    virtual const char *what() const throw()
    {
      return _msg.c_str();
    };
  };
}  // namespace bose_commserver
#endif  // NOLOGGEREXCEPTION_HPP
