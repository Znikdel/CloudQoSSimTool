#include "RunningVM.h"

void RunningVM::initialize(){
    userID =0;
           vm = NULL;
        //   vmID.clear();
           start_time=0;
           end_time=0;
}
RunningVM::RunningVM(){
    userID =0;
           vm = NULL;
        //   vmID.clear();
           start_time=0;
           end_time=0;
           hostNodeVL=NULL;
}
RunningVM::~RunningVM(){
    vm->finish();
}
