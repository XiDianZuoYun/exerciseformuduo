#ifndef COMMONEXCEPTION_H
#define COMMONEXCEPTION_H
#include <exception>
#include <string>
#define CHECK_COND(cond,msg,type)\
    if(!cond) \
        throw CommonException(msg,type);
class CommonException:public std::exception
{
public:
    enum ErrType{
        SOCKETErr=0,
        FILEEXCErr=1,
        POLLERErr=2,
        TIMERErr=3,
        BUFFERErr=4
    } ;
public:
    CommonException(const std::string& msg_, ErrType type) throw();
    ~CommonException() noexcept=default;
    CommonException()=delete;
    CommonException(const CommonException& other)=default;
    CommonException& operator=(const CommonException& other)=delete;
    virtual const char* what() const noexcept override
    {
        return msg.c_str();
    }
private:
    ErrType etype;
    std::string msg;
};

#endif // COMMONEXCEPTION_H
