#include<iostream>
#include<pthread.h>
#include<queue>
#include<functional>
#include<memory>
using std::vector;
typedef struct {
	pthread_cond_t cont;
	pthread_mutex_t mutex_;
} condition;
class	THREADPOOL{
private:
	condition _condtion;
	size_t maxsize=8;
	std::queue<std::function<void*()>(void *)> taskqueue;
	bool running_=false;
	std::vector<std::shared_ptr<pthread_t>> v;
public:
	void Init();
};

