//
//  AutoPathG.h
//  AutoPathG
//
//  Created by Kowalski on 12/22/13.
//
//

#ifndef __AutoPathA__AutoPathA__
#define __AutoPathA__AutoPathA__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
/*
 #include AutoPathG.h
 
 class MyClass :public AutoPathGDelegate{...}
 
 AutoPathG *path_finder;
    path_finder=AutoPathG::alloc();
    path_finder->retain();
    path_finder->init(CCTMXTidleMap->getMapSize...., this);
    path_finder->setFindPoints(ccp(2,8),ccp(7,6));
    
 get Result to process:
 
 delegate:
 AutoPathGdidFindPath(ccArray* path,bool secussed){
        if(secussed==false){
                dead end....
                return..
        }
        ccsquence......
        CCObject *obj;
        cc_array_for_each(path,obj){
            pathInformation *path_info;
                path_info=(pathInformation*)obj;
                ccsquence..add ccMoveTo(  convertGID to openGL location (path_info->getPoint()  ...
        }
 
        ccsprte -> runAction (ccsequence.....
}
 
 end:.
 
 cc_safr_release_null( path_finder)l
 */
class pathInformation:public CCObject{
    friend class AutoPathA;
    CCPoint m_point;
public:
    CCPoint getPoint(){
        return m_point;
    }
};

class AutoPathADelegate{
public:
    //ask you is here empty:
    //retuen ture means empty
            //false means here is wall or some thing sort of.
    virtual bool AutoPathAIsEmpty(CCPoint grid_point)
    {CC_UNUSED_PARAM(grid_point);return true;}
    virtual void AutoPathAdidFindPath(CCArray* path,bool secussed)
    {CC_UNUSED_PARAM(path);CC_UNUSED_PARAM(secussed);}
};

//Dot not use this class you self !
class __AutoPathA_chunk:public CCObject{
    friend class AutoPathA;
    friend class AutoPathADelegate;
    typedef enum{
        state_open,
        state_close
    } state;

    CCPoint m_GID;
    int m_step;
    float m_f_value;
    __AutoPathA_chunk* m_father;
    state m_state;
    
    
};

class AutoPathA:public CCObject{
public:
    static const unsigned int version=0x1;
    static AutoPathA* alloc();
    void init(CCSize mapSize,AutoPathADelegate* delegate);
    void setFindPoints(CCPoint start_point,CCPoint end_point);
private:
    //AutoPathG();
    CCSize m_total_size;
    AutoPathADelegate* m_delegate;
    __AutoPathA_chunk* m_last_closed;
    CCPoint m_start_point;
    CCPoint m_end_point;
    CCArray *m_chunks;
    void start_search();
    void begin_loop();
    __AutoPathA_chunk* get_chunck_of_min_f_value();
    bool is_reach_end_point(__AutoPathA_chunk* ch);
    void process_end(__AutoPathA_chunk* ch);
    bool ask_gid(CCPoint gid);
    void write_chunk(__AutoPathA_chunk* father,CCPoint gid);
    ~AutoPathA();
    
};
#endif /* defined(__AutoPathG__AutoPathG__) */
