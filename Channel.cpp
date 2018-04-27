#include "Channel.hpp"

void Channel::handleEvent()
{
  if (events_&EVENTIN) {
    readCallback();
  }
  if (events_&EVENTOUT) {
    writeCallback();
  }
  if(events_&EVENTERR){
    errorCallback();
  }
  handling_=false;
}
std::vector<Channel::functor> Channel::getCallBack()
{
    std::vector<functor> ret;
    if(events_&EVENTIN)
        ret.push_back(readCallback);
    if(events_&EVENTOUT)
        ret.push_back(writeCallback);
    if(events_&EVENTERR)
        ret.push_back(errorCallback);
    return ret;
}
int Channel::getfd()
{
  return fd_;
}
bool Channel::ishandling_()
{
  return handling_;
}

