//--- Licence -------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>

#ifndef __LOC_CURVE_H
#define __LOG_CURVE_H

#include  <stdlib.h>
#include "stm8s_conf.h"
#include "config.h"

#ifdef DIM_CURVE_TYPE_LOG

const uint16_t dim_curve[] = {288,288,288,288,289,289,289,289,289,289,290,290,290,290,290,291,291,291,291,292,292,292,292,292,293,
293,293,294,294,294,294,295,295,295,296,296,296,297,297,297,298,298,298,299,299,299,300,300,301,301,302,302,302,303,303,304,304,305,
305,306,307,307,308,308,309,309,310,311,311,312,313,313,314,315,316,316,317,318,319,320,320,321,322,323,324,325,326,327,328,329,330,
331,332,333,334,335,337,338,339,340,342,343,344,346,347,349,350,352,353,355,356,358,360,362,363,365,367,369,371,373,375,377,379,381,
383,385,388,390,393,395,397,400,403,405,408,411,414,417,420,423,426,429,432,435,439,442,446,449,453,457,461,465,469,473,477,481,486,
490,495,500,504,509,514,519,525,530,536,541,547,553,559,565,571,577,584,590,597,604,611,619,626,634,641,649,657,666,674,683,691,701,
710,719,729,739,749,759,769,780,791,802,814,825,837,850,862,875,888,901,915,929,943,958,972,988,1003,1019,1035,1052,1069,1086,1104,
1122,1140,1159,1178,1198,1218,1239,1260,1281,1303,1326,1349,1372,1396,1421,1446,1471,1497,1524,1552,1579,1608,1637,1667,1697,1728,
1760,1793,1826,1860,1895,1930,1966,2003,2041};

#endif /* DIM_CURVE_TYPE_LOG */
#endif /* __LOG_CURVE_H */
