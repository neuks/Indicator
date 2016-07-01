/*****************************************************************************
 * 禅论可视化分析系统
 * Copyright (C) 2016, Martin Tang

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef __FXIndicator_h__
#define __FXIndicator_h__
#pragma pack(push,1)

#include <windows.h>

// 函数(数据个数,输出,输入a,输入b,输入c)
typedef void(*pPluginFUNC)(int nCount, float *pOut, float *a, float *b, float *c);

typedef struct tagPluginTCalcFuncInfo
{
  unsigned short nFuncMark; // 函数编号
  pPluginFUNC    pCallFunc; // 函数地址
} PluginTCalcFuncInfo;

#ifdef __cplusplus
extern "C" {
#endif
DECLSPEC_EXPORT  BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo);  
#ifdef __cplusplus
};
#endif

#pragma pack(pop)
#endif
