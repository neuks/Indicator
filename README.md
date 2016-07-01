# 简介

缠论可视化交易插件，开发群：433343308

本插件自动画缠中说缠笔、中枢、线段斜率，带K线合并优化，可用于自行开发选股，全新主流的算法速度杠杠的。喜欢缠论的朋友一起来战斗吧！

#风险警示

本软件仅旨在实现最贴近缠师原文所的线段、中枢概念的可视化结果，软件免费分享使用，没有任何限制。

市场有风险，使用者自行承担任何由本软件而导致的买卖交易后果，作者本人不承担因为使用本软件而导致的任何直接或间接后果。

#安装方法

目前仅支持通达信软件使用。安装时，首先将CZSC.dll复制到通达信安装目录下的T0002\dlls目录之中，并在通达信公式管理器中将本dll加载到1号dll插件之中。

#通达信端代码

```text
DLL:=TDXDLL1(1,H,L,5);
HIB:=TDXDLL1(2,DLL,H,L);
LOB:=TDXDLL1(3,DLL,H,L);
SIG:=TDXDLL1(4,DLL,H,L);
BS1:=TDXDLL1(5,DLL,H,L);
BS2:=TDXDLL1(6,DLL,H,L);
STH:=TDXDLL1(7,DLL,H,L);
SLP:=TDXDLL1(8,DLL,H,L);
IF(HIB,HIB,DRAWNULL), COLORYELLOW;
IF(LOB,LOB,DRAWNULL), COLORYELLOW;
STICKLINE(SIG,LOB,HIB,0,0), COLORYELLOW;
DRAWLINE(DLL=-1,L,DLL=+1,H,0), COLORYELLOW;
DRAWLINE(DLL=+1,H,DLL=-1,L,0), COLORYELLOW;
DRAWNUMBER(DLL=+1,H,STH), COLORYELLOW, DRAWABOVE;
DRAWNUMBER(DLL=-1,L,STH), COLORYELLOW;
BUY(BS1=3,LOW);
SELL(BS1=12,HIGH);
BUYSHORT(BS1=2,LOW);
SELLSHORT(BS1=13,HIGH);
```

#联系方式

欲得到更多功能，欢迎加入：
缠论算法交易群：433343308 (QQ)

加群验证码发送：CZSC
