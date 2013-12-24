//
//  AutoPathG.cpp
//  AutoPathG
//
//  Created by Kowalski on 12/22/13.
//
//

#include "AutoPathA.h"

AutoPathA* AutoPathA::alloc(){
    AutoPathA *path_finder=new AutoPathA();
    path_finder->autorelease();
    return path_finder;
}

void AutoPathA::init(cocos2d::CCSize mapSize, AutoPathADelegate *delegate){
    m_delegate=delegate;
    m_total_size=mapSize;
}

void AutoPathA::setFindPoints(cocos2d::CCPoint start_point, cocos2d::CCPoint end_point){
    m_start_point=start_point;
    m_end_point=end_point;
    CC_SAFE_RELEASE_NULL(m_chunks);
    m_chunks=CCArray::create();
    m_chunks->retain();
    write_chunk(NULL, m_start_point);
    begin_loop();
}

void AutoPathA::write_chunk(__AutoPathA_chunk *father, cocos2d::CCPoint gid){
    __AutoPathA_chunk* ch=new __AutoPathA_chunk();
    ch->autorelease();
    ch->m_father=father;
    ch->m_GID=gid;
    if (ch->m_father==NULL) {
        ch->m_step=0;
    }else{
        ch->m_step=ch->m_father->m_step+1;
    }
    CCPoint g_value;
    g_value=ccpSub(ch->m_GID, m_end_point);
    ch->m_f_value=fabsf(g_value.x)+fabsf(g_value.y)+ch->m_step;
    ch->m_state=__AutoPathA_chunk::state_open;
    m_chunks->addObject(ch);
}

void AutoPathA::begin_loop(){
    while (true) {
        __AutoPathA_chunk* working_chunk;
        working_chunk=this->get_chunck_of_min_f_value();
        
        //check dead end
        if (working_chunk==NULL) {
            this->m_delegate->AutoPathAdidFindPath(NULL, false);
            return;
        }
        
        //check reach end point
        if (working_chunk->m_GID.equals(m_end_point)) {
            process_end(working_chunk);
            return;
        }
        
        //ask each gid around chunk
        
        CCPoint offsets[]={
            CCPoint(-1.0f,+1.0f),
            CCPoint(0.0f,+1.0f),
            CCPoint(+1.0f, +1.0f),
            CCPoint(-1.0f,0.0f),
            CCPoint(+1.0f,0.0f),
            CCPoint(-1.0f,-1.0f),
            CCPoint(0.0f,-1.0f),
            CCPoint(+1.0f,-1.0f)
        };

        for (int i=0;i<8; i++) {
            CCPoint tmp_gid=ccpAdd(working_chunk->m_GID,offsets[i]);
            bool can_write=ask_gid(tmp_gid);
            if (can_write) {
                write_chunk(working_chunk, tmp_gid);
            }
        }
        working_chunk->m_state=__AutoPathA_chunk::state_close;
        m_last_closed=working_chunk;
        
    }
}

bool AutoPathA::ask_gid(cocos2d::CCPoint gid){
    
    if (gid.x>m_total_size.width-1||gid.x<0||gid.y>m_total_size.height-1||gid.y<0) {
        return false;
    }
    
    if (this->m_delegate->AutoPathAIsEmpty(gid)==false) {
        return false;
    }
    
    CCObject *obj=NULL;
    CCARRAY_FOREACH(m_chunks, obj){
        __AutoPathA_chunk *tmp_ch=(__AutoPathA_chunk* )obj;
        if (tmp_ch->m_GID.equals(gid)) {
            return false;
        }
    }
    
    return true;
}

__AutoPathA_chunk* AutoPathA::get_chunck_of_min_f_value(){
    __AutoPathA_chunk *min_f_v_ch=NULL;
    CCObject *obj=NULL;
    CCARRAY_FOREACH(m_chunks, obj){
        __AutoPathA_chunk *tmp_ch=(__AutoPathA_chunk* )obj;
        if (tmp_ch->m_state==__AutoPathA_chunk::state_close) {
            continue;
        }
        if (min_f_v_ch==NULL) {
            min_f_v_ch=tmp_ch;
            continue;
        }
        if (tmp_ch->m_state==__AutoPathA_chunk::state_close) {
            continue;
        }
        if (tmp_ch->m_f_value<min_f_v_ch->m_f_value) {
            min_f_v_ch=tmp_ch;
        }else if(tmp_ch->m_f_value==min_f_v_ch->m_f_value) {
            if (m_last_closed!=NULL) {
                if (ccpDistance(tmp_ch->m_GID, m_last_closed->m_GID)<ccpDistance(min_f_v_ch->m_GID, m_last_closed->m_GID)) {
                    min_f_v_ch=tmp_ch;
                }
            }else{
                min_f_v_ch=tmp_ch;
            }
        }
    }
    return min_f_v_ch;
}

void AutoPathA::process_end(__AutoPathA_chunk *ch){

    CCLog("sizeof each:%lu num:%i",sizeof(__AutoPathA_chunk),m_chunks->count());
    CCArray *final_result=CCArray::create();
    do {
        pathInformation *info=new pathInformation();
        info->autorelease();
        info->m_point=ccp(ch->m_GID.x, ch->m_GID.y);
        final_result->addObject(info);
        CCLog("x:%0.0f,y:%0.0f step:%i f_v:%0.0f",ch->m_GID.x,ch->m_GID.y,ch->m_step,ch->m_f_value);
        ch=ch->m_father;
    } while (ch->m_father!=NULL);
    m_delegate->AutoPathAdidFindPath(final_result, true);
}

AutoPathA::~AutoPathA(){
    CC_SAFE_RELEASE_NULL(m_chunks);
}