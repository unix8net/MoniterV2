#ifndef _UTILITY_H_
#define _UTILITY_H_
/*==================================================================
 *    作者：Creater
 *    简介：该类为工具模板类
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/
class Utility
{
public:
    template<typename T> static void DESTORY_PTR(T t)
    {
        if(t != 0)
        {
            delete t;
            t = 0;
        }
    }
    template<typename T> static void DESTORY_ARR(T t)
    {
        if(t != 0)
        {
            delete []t;
            t = 0;
        }
    }

};
#endif
