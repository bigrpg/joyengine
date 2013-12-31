//
//  StructInfo.h
//  connection
//
//  Created by 李 启虎 on 13-7-4.
//  Copyright (c) 2013年 joygame. All rights reserved.
//

#ifndef connection_StructInfo_h
#define connection_StructInfo_h

#include  "stdinc/stdinc.h"

struct HeroInfo
{
    uint16    uuid;
    uint16    no;         // 英雄编号
    uint16    level;      // 等级
    uint32    exp;        // 经验
    uint8     skillNo;    // 技能编号
};


#endif
