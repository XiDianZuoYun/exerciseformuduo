#include <string.h>
#include <error.h>
#include "commonexception.h"

CommonException::CommonException(const std::string& msg_,ErrType type) throw():msg(msg_),etype(type),error_code(errno)
{
    switch (type) {
    case ErrType::SOCKETErr:
        msg.append(" SOCKET ERROR:");
        msg.append(strerror(errno));
        break;
    case ErrType::FILEEXCErr:
        msg.append(" FILE ERROR:");
        msg.append(strerror(errno));
        break;
    case ErrType::POLLERErr:
        msg.append(" POLLER ERROR:");
        msg.append(strerror(errno));
        break;
    case ErrType::TIMERErr:
        msg.append(" TIMER ERROR:");
        msg.append(strerror(errno));
    case ErrType::BUFFERErr:
        msg.append(" BUFFER ERROR:");
        msg.append(strerror(errno));
    default:
        break;
    }
}
