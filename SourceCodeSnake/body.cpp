#include "body.h"

Body::Body()
{
    x=0;
    y=0;
    next=nullptr;
}
Body::Body(int h,int v){
    x=h;
    y=v;
    next=nullptr;
}
Body::~Body(){
    ;
}
