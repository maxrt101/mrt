#include "job.h"

#include <iostream>
#include <mutex>

mrt::threads::Job::Job() {}

mrt::threads::Job::Job(JobFunction function, void* arg) 
    : function(function), arg(arg) {}

void mrt::threads::Job::Run() {
  function(arg);
}


