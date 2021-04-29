#ifndef BODY_H
#define BODY_H


class Body
{
public:
    Body();
    Body(int h,int v);
    ~Body();\
public:
    int x;
    int y;
    Body* next;
};

#endif // BODY_H
