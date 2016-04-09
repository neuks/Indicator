/*****************************************************************************
 * Visual K-Line Analysing System For Zen Theory
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

#include "Main.h"

//=============================================================================
// Mathmatic Functions
//=============================================================================

// Find top & bottom
void Parse1(int nCount, float *pOut, float *pHigh, float *pLow)
{
  int nState = -1;
  int nHigh  = 0;
  int nLow   = 0;

  for (int i = 1; i < nCount; i++)
  {
    // Set default value
    pOut[i] = 0;

    // Seeking high points
    if (nState == 1)
    {
      // Set initial high index, and update at higher points
      if (pHigh[i] >= pHigh[nHigh])
      {
        pOut[nHigh] = 0;
        nHigh = i;
        pOut[nHigh] = 1;
      }

      // Confirm turn, switch state and return
      if ((pHigh[i] < pHigh[nHigh]) && (pLow[i]  < pLow[nHigh]))
      {
        pOut[nHigh] = 1;

        nState = -1;
        nLow   = i;
      }
    }

    // Seeking low points
    else if (nState == -1)
    {
      // Set initial low index, and update at lower points
      if (pLow[i] <= pLow[nLow])
      {
        pOut[nLow] = 0;
        nLow = i;
        pOut[nLow] = -1;
      }

      // Confirm turn, switch state and return
      if ((pLow[i]  > pLow[nLow]) && (pHigh[i] > pHigh[nLow]))
      {
        pOut[nLow] = -1;

        nState = 1;
        nHigh  = i;
      }
    }
  }
}

// Simlification
int Parse2(int nCount, float *pOut, float *pHigh, float *pLow)
{
  int nCurrTop = 0, nPrevTop = 0;
  int nCurrBot = 0, nPrevBot = 0;

  // Simplify connections using merging method
  for (int i = 0; i < nCount; i++)
  {
    // Setting up referencing indexes
    if (pOut[i] == 1)
    {
      nPrevTop = nCurrTop;
      nCurrTop = i;
    }
    else if (pOut[i] == -1)
    {
      nPrevBot = nCurrBot;
      nCurrBot = i;
    }

    // Merging rising trends
    if (pOut[i] == 1)
    {
      // Check if period is invalid
      if (((nCurrTop - nCurrBot < 4) && (nCount   - nCurrTop > 4)) ||
           (nCurrBot - nPrevTop < 4) || (nPrevTop - nPrevBot < 4))
      {
        // On Up-Down-Up patterns, merge down link
        if ((pHigh[nCurrTop] >= pHigh[nPrevTop]) &&
            (pLow [nCurrBot] >  pLow [nPrevBot]))
        {
          pOut[nCurrBot] = 0;
          pOut[nPrevTop] = 0;
        }
      }
    }

    // Merging falling trends
    if (pOut[i] == -1)
    {
      // Check if period is invalid
      if (((nCurrBot - nCurrTop < 4) && (nCount   - nCurrBot > 4)) ||
           (nCurrTop - nPrevBot < 4) || (nPrevBot - nPrevTop < 4))
      {
        // On Down-Up-Down patterns, merge up link
        if ((pLow [nCurrBot] <= pLow [nPrevBot]) &&
            (pHigh[nCurrTop] <  pHigh[nPrevTop]))
        {
          pOut[nCurrTop] = 0;
          pOut[nPrevBot] = 0;
        }
      }
    }
  }
}

//=============================================================================
// Func1: Output Link Information
//=============================================================================

void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pTime)
{
  Parse1(nCount, pOut, pHigh, pLow);

  for (int i = 0; i < *pTime; i++)
  {
    Parse2(nCount, pOut, pHigh, pLow);
  }
}

//=============================================================================
// Func2: Output Centroid High
//=============================================================================

void Func2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // Push state and check if centroid finishes
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPHigh;
        }
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]))
      {
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPHigh;
        }
      }
    }

    // Output tailing centroid
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      for (int j = Centroid.nStart; j < nCount; j++)
      {
        pOut[j] = Centroid.fHigh;
      }
    }
  }
}

//=============================================================================
// Func3: Output Centroid Low
//=============================================================================

void Func3(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // Push state and check if centroid finishes
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPLow;
        }
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]))
      {
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPLow;
        }
      }
    }

    // Output tailing centroid
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      for (int j = Centroid.nStart; j < nCount; j++)
      {
        pOut[j] = Centroid.fLow;
      }
    }
  }
}

//=============================================================================
// Func4: Output Centroid Start & End Points
//=============================================================================

void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // Output centroid start and end signals
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        pOut[Centroid.nStart] = 1;
        pOut[Centroid.nEnd]   = 2;
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]))
      {
        pOut[Centroid.nStart] = 1;
        pOut[Centroid.nEnd]   = 2;
      }
    }

    // Output tailing centroid signal
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      pOut[Centroid.nStart] = 1;
      pOut[nCount-1]        = 2;
    }
  }
}

//=============================================================================
// Func5: Output Visual Buy/Sell Points
//=============================================================================

void Func5(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // Output centroid start and end signals
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        // 3rd sell point
        pOut[i] = 13;
      }
      else if (Centroid.fTop1 < Centroid.fTop2)
      {
        // 2nd sell point
        pOut[i] = 12;
      }
      else
      {
        // no sell point
        pOut[i] = 0;
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]))
      {
        // 3rd buy point
        pOut[i] = 3;
      }
      else if (Centroid.fBot1 > Centroid.fBot2)
      {
        // 2nd buy point
        pOut[i] = 2;
      }
      else
      {
        // no buy point
        pOut[i] = 0;
      }
    }
  }
}

//=============================================================================
// Func6: Output Operational Buy/Sell Points
//=============================================================================

void Func6(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // Output centroid start and end signals
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]) && (Centroid.fBot1 < Centroid.fBot2))
      {
        // 3rd sell point
        pOut[i] = 13;
        pOut[nCount-1] = 13;
      }
      else if ((Centroid.fTop1 < Centroid.fTop2) &&
               (Centroid.fBot1 < Centroid.fBot2) &&
               (Centroid.fTop1 > Centroid.fBot2))
      {
        // 2nd sell point
        pOut[i] = 12;
        pOut[nCount-1] = 12;
      }
      else
      {
        // no sell point
        pOut[i] = 0;
        pOut[nCount-1] = 0;
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]) && (Centroid.fTop1 > Centroid.fTop2))
      {
        // 3rd buy point
        pOut[i] = 3;
        pOut[nCount-1] = 3;
      }
      else if ((Centroid.fBot1 > Centroid.fBot2) &&
               (Centroid.fTop1 > Centroid.fTop2) &&
               (Centroid.fBot1 < Centroid.fTop2))
      {
        // 2nd buy point
        pOut[i] = 2;
        pOut[nCount-1] = 2;
      }
      else
      {
        // no buy point
        pOut[i] = 0;
        pOut[nCount-1] = 0;
      }
    }
  }
}

//=============================================================================
// Func7: Output Trend Strength Indicator
//=============================================================================

void Func7(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  int nStatus = 0, nPrevTop, nPrevBot;

  for (int i = 0; i < nCount; i++)
  {
    // switch calculation state
    if (pIn[i-1] == 1)
    {
      nStatus  = -1;
      nPrevTop = i - 1;
    }
    else if (pIn[i-1] == -1)
    {
      nStatus  = 1;
      nPrevBot = i - 1;
    }

    if (nStatus == 1)
    {
      // calculate for rising pattern
      pOut[i] = (pHigh[i] - pLow[nPrevBot]) / (i - nPrevBot)
        / pLow[nPrevBot] * 100;
    }
    else if (nStatus == -1)
    {
      // calculate for falling pattern
      pOut[i] = (pLow[i] - pHigh[nPrevTop]) / (i - nPrevTop)
        / pHigh[nPrevTop] * 100;
    }
  }
}

static PluginTCalcFuncInfo Info[] =
{
  {1, &Func1},
  {2, &Func2},
  {3, &Func3},
  {4, &Func4},
  {5, &Func5},
  {6, &Func6},
  {7, &Func7},
  {0, NULL},
};

BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo)
{
  if (*pInfo == NULL)
  {
    *pInfo = Info;

    return TRUE;
  }

  return FALSE;
}

