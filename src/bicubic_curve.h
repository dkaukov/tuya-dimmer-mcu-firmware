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

#ifndef __BICUBIC_CURVE_H
#define __BICUBIC_CURVE_H

#include  <stdlib.h>
#include "stm8s_conf.h"
#include "config.h"

#ifdef DIM_CURVE_TYPE_BICUBIC

#define pwrtwo(x) (float)(1 << (x))
#define POINT(X)  ((DIM_CURVE_MIN_VAL + ((1 - DIM_CURVE_BICUBIC_EXP_FACTOR)*((float)X/255.0)*((float)X/255.0)*((float)X/255.0) + DIM_CURVE_BICUBIC_EXP_FACTOR*((float)X/255.0))*(255.0 - DIM_CURVE_MIN_VAL)) * pwrtwo(DIM_CURVE_FP_BITS))

const uint16_t dim_curve[256] = {
  POINT(0),
  POINT(1),
  POINT(2),
  POINT(3),
  POINT(4),
  POINT(5),
  POINT(6),
  POINT(7),
  POINT(8),
  POINT(9),
  POINT(10),
  POINT(11),
  POINT(12),
  POINT(13),
  POINT(14),
  POINT(15),
  POINT(16),
  POINT(17),
  POINT(18),
  POINT(19),
  POINT(20),
  POINT(21),
  POINT(22),
  POINT(23),
  POINT(24),
  POINT(25),
  POINT(26),
  POINT(27),
  POINT(28),
  POINT(29),
  POINT(30),
  POINT(31),
  POINT(32),
  POINT(33),
  POINT(34),
  POINT(35),
  POINT(36),
  POINT(37),
  POINT(38),
  POINT(39),
  POINT(40),
  POINT(41),
  POINT(42),
  POINT(43),
  POINT(44),
  POINT(45),
  POINT(46),
  POINT(47),
  POINT(48),
  POINT(49),
  POINT(50),
  POINT(51),
  POINT(52),
  POINT(53),
  POINT(54),
  POINT(55),
  POINT(56),
  POINT(57),
  POINT(58),
  POINT(59),
  POINT(60),
  POINT(61),
  POINT(62),
  POINT(63),
  POINT(64),
  POINT(65),
  POINT(66),
  POINT(67),
  POINT(68),
  POINT(69),
  POINT(70),
  POINT(71),
  POINT(72),
  POINT(73),
  POINT(74),
  POINT(75),
  POINT(76),
  POINT(77),
  POINT(78),
  POINT(79),
  POINT(80),
  POINT(81),
  POINT(82),
  POINT(83),
  POINT(84),
  POINT(85),
  POINT(86),
  POINT(87),
  POINT(88),
  POINT(89),
  POINT(90),
  POINT(91),
  POINT(92),
  POINT(93),
  POINT(94),
  POINT(95),
  POINT(96),
  POINT(97),
  POINT(98),
  POINT(99),
  POINT(100),
  POINT(101),
  POINT(102),
  POINT(103),
  POINT(104),
  POINT(105),
  POINT(106),
  POINT(107),
  POINT(108),
  POINT(109),
  POINT(110),
  POINT(111),
  POINT(112),
  POINT(113),
  POINT(114),
  POINT(115),
  POINT(116),
  POINT(117),
  POINT(118),
  POINT(119),
  POINT(120),
  POINT(121),
  POINT(122),
  POINT(123),
  POINT(124),
  POINT(125),
  POINT(126),
  POINT(127),
  POINT(128),
  POINT(129),
  POINT(130),
  POINT(131),
  POINT(132),
  POINT(133),
  POINT(134),
  POINT(135),
  POINT(136),
  POINT(137),
  POINT(138),
  POINT(139),
  POINT(140),
  POINT(141),
  POINT(142),
  POINT(143),
  POINT(144),
  POINT(145),
  POINT(146),
  POINT(147),
  POINT(148),
  POINT(149),
  POINT(150),
  POINT(151),
  POINT(152),
  POINT(153),
  POINT(154),
  POINT(155),
  POINT(156),
  POINT(157),
  POINT(158),
  POINT(159),
  POINT(160),
  POINT(161),
  POINT(162),
  POINT(163),
  POINT(164),
  POINT(165),
  POINT(166),
  POINT(167),
  POINT(168),
  POINT(169),
  POINT(170),
  POINT(171),
  POINT(172),
  POINT(173),
  POINT(174),
  POINT(175),
  POINT(176),
  POINT(177),
  POINT(178),
  POINT(179),
  POINT(180),
  POINT(181),
  POINT(182),
  POINT(183),
  POINT(184),
  POINT(185),
  POINT(186),
  POINT(187),
  POINT(188),
  POINT(189),
  POINT(190),
  POINT(191),
  POINT(192),
  POINT(193),
  POINT(194),
  POINT(195),
  POINT(196),
  POINT(197),
  POINT(198),
  POINT(199),
  POINT(200),
  POINT(201),
  POINT(202),
  POINT(203),
  POINT(204),
  POINT(205),
  POINT(206),
  POINT(207),
  POINT(208),
  POINT(209),
  POINT(210),
  POINT(211),
  POINT(212),
  POINT(213),
  POINT(214),
  POINT(215),
  POINT(216),
  POINT(217),
  POINT(218),
  POINT(219),
  POINT(220),
  POINT(221),
  POINT(222),
  POINT(223),
  POINT(224),
  POINT(225),
  POINT(226),
  POINT(227),
  POINT(228),
  POINT(229),
  POINT(230),
  POINT(231),
  POINT(232),
  POINT(233),
  POINT(234),
  POINT(235),
  POINT(236),
  POINT(237),
  POINT(238),
  POINT(239),
  POINT(240),
  POINT(241),
  POINT(242),
  POINT(243),
  POINT(244),
  POINT(245),
  POINT(246),
  POINT(247),
  POINT(248),
  POINT(249),
  POINT(250),
  POINT(251),
  POINT(252),
  POINT(253),
  POINT(254),
  POINT(255)
};
#endif /* DIM_CURVE_TYPE_BICUBIC */
#endif /* __BICUBIC_CURVE_H */
