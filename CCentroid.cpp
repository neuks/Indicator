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

#include "CCentroid.h"

CCentroid::CCentroid()
{
  this->bValid = false;
  this->nTop1  = 0;
  this->nTop2  = 0;
  this->nBot1  = 0;
  this->nBot2  = 0;
  this->fTop1  = 0;
  this->fTop2  = 0;
  this->fBot1  = 0;
  this->fBot2  = 0;
  this->nLines = 0;
  this->nStart = 0;
  this->nEnd   = 0;
  this->fHigh  = 0;
  this->fLow   = 0;
  this->fPHigh = 0;
  this->fPLow  = 0;
}

CCentroid::~CCentroid()
{
}

// 推入高点并计算状态
bool CCentroid::PushHigh(int nIndex, float fValue)
{
  if (bValid == true)
  {
    nLines++;
    fPHigh = fHigh;
    fPLow  = fLow;
  }
  else
  {
    nLines = 0;
  }

  // 更新高低点位置信息
  nTop2 = nTop1;
  fTop2 = fTop1;
  nTop1 = nIndex;
  fTop1 = fValue;

  // 如果非中枢模式下
  if (bValid == false)
  {
    // 更新中枢高
    if (fTop1 < fTop2)
    {
      fHigh  = fTop1;
    }
    else
    {
      fHigh  = fTop2;
    }

    // 中枢识别
    if (fHigh > fLow)
    {
      if  (fBot1 < fBot2)
      {
        nStart = nBot2;
      }
      else
      {
        nStart = nTop2;
      }
      bValid = true;
    }
  }
  // 如果在中枢中
  else
  {
    // 更新中枢高
    if (fHigh > fTop1)
    {
      fHigh  = fTop1;
    }

    // 中枢终结
    if (fHigh < fLow)
    {
      fHigh  = fTop1;
      fLow   = fBot1;
      nEnd   = nTop2;
      bValid = false;

      if (nLines > 2)
      {
        return true;
      }
    }
  }

  return false;
}

bool CCentroid::PushLow(int nIndex, float fValue)
{
  if (bValid == true)
  {
    nLines++;
    fPLow  = fLow;
    fPHigh = fHigh;
  }
  else
  {
    nLines = 0;
  }

  // 更新定位信息
  nBot2 = nBot1;
  fBot2 = fBot1;
  nBot1 = nIndex;
  fBot1 = fValue;

  // 如果非中枢模式下
  if (bValid == false)
  {
    // 更新区间低点
    if (fBot1 > fBot2)
    {
      fLow = fBot1;
    }
    else
    {
      fLow = fBot2;
    }

    // 中枢捕捉
    if (fHigh > fLow)
    {
      if (fTop1 > fTop2)
      {
        nStart = nTop2;
      }
      else
      {
        nStart = nBot2;
      }
      bValid = true;
    }
  }
  // 如果在中枢中
  else 
  {
    // 更新中枢低
    if (fLow < fBot1)
    {
      fLow = fBot1;
    }

    // 中枢终结
    if (fHigh < fLow)
    {
      fHigh  = fTop1;
      fLow   = fBot1;
      nEnd   = nBot2;
      bValid = false;

      if (nLines > 2)
      {
        return true;
      }
    }
  }

  return false;
}
