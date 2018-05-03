/*
 * sin.c
 *
 *  Created on: Apr 30, 2018
 *      Author: Nick
 */
#include "sin.h"

uint16_t sinUpdate(uint16_t time)
{
    static const uint16_t lookup[SINEPOINTS] = { 2048, 2056, 2064, 2072, 2080,
                                                 2088, 2096, 2104, 2112, 2120,
                                                 2128, 2136, 2144, 2152, 2160,
                                                 2168, 2176, 2184, 2192, 2200,
                                                 2208, 2216, 2224, 2232, 2240,
                                                 2248, 2256, 2264, 2272, 2280,
                                                 2288, 2296, 2304, 2312, 2320,
                                                 2328, 2336, 2344, 2352, 2360,
                                                 2368, 2376, 2384, 2392, 2400,
                                                 2408, 2415, 2423, 2431, 2439,
                                                 2447, 2455, 2463, 2471, 2479,
                                                 2486, 2494, 2502, 2510, 2518,
                                                 2526, 2533, 2541, 2549, 2557,
                                                 2565, 2572, 2580, 2588, 2596,
                                                 2603, 2611, 2619, 2627, 2634,
                                                 2642, 2650, 2657, 2665, 2673,
                                                 2680, 2688, 2696, 2703, 2711,
                                                 2718, 2726, 2734, 2741, 2749,
                                                 2756, 2764, 2771, 2779, 2786,
                                                 2794, 2801, 2809, 2816, 2824,
                                                 2831, 2839, 2846, 2853, 2861,
                                                 2868, 2876, 2883, 2890, 2898,
                                                 2905, 2912, 2919, 2927, 2934,
                                                 2941, 2948, 2956, 2963, 2970,
                                                 2977, 2984, 2992, 2999, 3006,
                                                 3013, 3020, 3027, 3034, 3041,
                                                 3048, 3055, 3062, 3069, 3076,
                                                 3083, 3090, 3097, 3104, 3111,
                                                 3118, 3124, 3131, 3138, 3145,
                                                 3152, 3158, 3165, 3172, 3179,
                                                 3185, 3192, 3199, 3205, 3212,
                                                 3219, 3225, 3232, 3238, 3245,
                                                 3251, 3258, 3264, 3271, 3277,
                                                 3284, 3290, 3296, 3303, 3309,
                                                 3315, 3322, 3328, 3334, 3341,
                                                 3347, 3353, 3359, 3365, 3371,
                                                 3378, 3384, 3390, 3396, 3402,
                                                 3408, 3414, 3420, 3426, 3432,
                                                 3438, 3444, 3449, 3455, 3461,
                                                 3467, 3473, 3478, 3484, 3490,
                                                 3496, 3501, 3507, 3513, 3518,
                                                 3524, 3529, 3535, 3540, 3546,
                                                 3551, 3557, 3562, 3568, 3573,
                                                 3578, 3584, 3589, 3594, 3600,
                                                 3605, 3610, 3615, 3620, 3626,
                                                 3631, 3636, 3641, 3646, 3651,
                                                 3656, 3661, 3666, 3671, 3676,
                                                 3680, 3685, 3690, 3695, 3700,
                                                 3704, 3709, 3714, 3718, 3723,
                                                 3728, 3732, 3737, 3741, 3746,
                                                 3750, 3755, 3759, 3764, 3768,
                                                 3772, 3777, 3781, 3785, 3789,
                                                 3794, 3798, 3802, 3806, 3810,
                                                 3814, 3818, 3822, 3826, 3830,
                                                 3834, 3838, 3842, 3846, 3850,
                                                 3854, 3857, 3861, 3865, 3869,
                                                 3872, 3876, 3880, 3883, 3887,
                                                 3890, 3894, 3897, 3901, 3904,
                                                 3907, 3911, 3914, 3917, 3921,
                                                 3924, 3927, 3930, 3933, 3937,
                                                 3940, 3943, 3946, 3949, 3952,
                                                 3955, 3958, 3960, 3963, 3966,
                                                 3969, 3972, 3974, 3977, 3980,
                                                 3982, 3985, 3988, 3990, 3993,
                                                 3995, 3998, 4000, 4003, 4005,
                                                 4007, 4010, 4012, 4014, 4016,
                                                 4019, 4021, 4023, 4025, 4027,
                                                 4029, 4031, 4033, 4035, 4037,
                                                 4039, 4041, 4043, 4044, 4046,
                                                 4048, 4050, 4051, 4053, 4055,
                                                 4056, 4058, 4059, 4061, 4062,
                                                 4064, 4065, 4066, 4068, 4069,
                                                 4070, 4072, 4073, 4074, 4075,
                                                 4076, 4077, 4078, 4079, 4080,
                                                 4081, 4082, 4083, 4084, 4085,
                                                 4086, 4086, 4087, 4088, 4089,
                                                 4089, 4090, 4090, 4091, 4091,
                                                 4092, 4092, 4093, 4093, 4094,
                                                 4094, 4094, 4094, 4095, 4095,
                                                 4095, 4095, 4095, 4095, 4095,
                                                 4096, 4095, 4095, 4095, 4095,
                                                 4095, 4095, 4095, 4094, 4094,
                                                 4094, 4094, 4093, 4093, 4092,
                                                 4092, 4091, 4091, 4090, 4090,
                                                 4089, 4089, 4088, 4087, 4086,
                                                 4086, 4085, 4084, 4083, 4082,
                                                 4081, 4080, 4079, 4078, 4077,
                                                 4076, 4075, 4074, 4073, 4072,
                                                 4070, 4069, 4068, 4066, 4065,
                                                 4064, 4062, 4061, 4059, 4058,
                                                 4056, 4055, 4053, 4051, 4050,
                                                 4048, 4046, 4044, 4043, 4041,
                                                 4039, 4037, 4035, 4033, 4031,
                                                 4029, 4027, 4025, 4023, 4021,
                                                 4019, 4016, 4014, 4012, 4010,
                                                 4007, 4005, 4003, 4000, 3998,
                                                 3995, 3993, 3990, 3988, 3985,
                                                 3982, 3980, 3977, 3974, 3972,
                                                 3969, 3966, 3963, 3960, 3958,
                                                 3955, 3952, 3949, 3946, 3943,
                                                 3940, 3937, 3933, 3930, 3927,
                                                 3924, 3921, 3917, 3914, 3911,
                                                 3907, 3904, 3901, 3897, 3894,
                                                 3890, 3887, 3883, 3880, 3876,
                                                 3872, 3869, 3865, 3861, 3857,
                                                 3854, 3850, 3846, 3842, 3838,
                                                 3834, 3830, 3826, 3822, 3818,
                                                 3814, 3810, 3806, 3802, 3798,
                                                 3794, 3789, 3785, 3781, 3777,
                                                 3772, 3768, 3764, 3759, 3755,
                                                 3750, 3746, 3741, 3737, 3732,
                                                 3728, 3723, 3718, 3714, 3709,
                                                 3704, 3700, 3695, 3690, 3685,
                                                 3680, 3676, 3671, 3666, 3661,
                                                 3656, 3651, 3646, 3641, 3636,
                                                 3631, 3626, 3620, 3615, 3610,
                                                 3605, 3600, 3594, 3589, 3584,
                                                 3578, 3573, 3568, 3562, 3557,
                                                 3551, 3546, 3540, 3535, 3529,
                                                 3524, 3518, 3513, 3507, 3501,
                                                 3496, 3490, 3484, 3478, 3473,
                                                 3467, 3461, 3455, 3449, 3444,
                                                 3438, 3432, 3426, 3420, 3414,
                                                 3408, 3402, 3396, 3390, 3384,
                                                 3378, 3371, 3365, 3359, 3353,
                                                 3347, 3341, 3334, 3328, 3322,
                                                 3315, 3309, 3303, 3296, 3290,
                                                 3284, 3277, 3271, 3264, 3258,
                                                 3251, 3245, 3238, 3232, 3225,
                                                 3219, 3212, 3205, 3199, 3192,
                                                 3185, 3179, 3172, 3165, 3158,
                                                 3152, 3145, 3138, 3131, 3124,
                                                 3118, 3111, 3104, 3097, 3090,
                                                 3083, 3076, 3069, 3062, 3055,
                                                 3048, 3041, 3034, 3027, 3020,
                                                 3013, 3006, 2999, 2992, 2984,
                                                 2977, 2970, 2963, 2956, 2948,
                                                 2941, 2934, 2927, 2919, 2912,
                                                 2905, 2898, 2890, 2883, 2876,
                                                 2868, 2861, 2853, 2846, 2839,
                                                 2831, 2824, 2816, 2809, 2801,
                                                 2794, 2786, 2779, 2771, 2764,
                                                 2756, 2749, 2741, 2734, 2726,
                                                 2718, 2711, 2703, 2696, 2688,
                                                 2680, 2673, 2665, 2657, 2650,
                                                 2642, 2634, 2627, 2619, 2611,
                                                 2603, 2596, 2588, 2580, 2572,
                                                 2565, 2557, 2549, 2541, 2533,
                                                 2526, 2518, 2510, 2502, 2494,
                                                 2486, 2479, 2471, 2463, 2455,
                                                 2447, 2439, 2431, 2423, 2415,
                                                 2408, 2400, 2392, 2384, 2376,
                                                 2368, 2360, 2352, 2344, 2336,
                                                 2328, 2320, 2312, 2304, 2296,
                                                 2288, 2280, 2272, 2264, 2256,
                                                 2248, 2240, 2232, 2224, 2216,
                                                 2208, 2200, 2192, 2184, 2176,
                                                 2168, 2160, 2152, 2144, 2136,
                                                 2128, 2120, 2112, 2104, 2096,
                                                 2088, 2080, 2072, 2064, 2056,
                                                 2048, 2039, 2031, 2023, 2015,
                                                 2007, 1999, 1991, 1983, 1975,
                                                 1967, 1959, 1951, 1943, 1935,
                                                 1927, 1919, 1911, 1903, 1895,
                                                 1887, 1879, 1871, 1863, 1855,
                                                 1847, 1839, 1831, 1823, 1815,
                                                 1807, 1799, 1791, 1783, 1775,
                                                 1767, 1759, 1751, 1743, 1735,
                                                 1727, 1719, 1711, 1703, 1695,
                                                 1687, 1680, 1672, 1664, 1656,
                                                 1648, 1640, 1632, 1624, 1616,
                                                 1609, 1601, 1593, 1585, 1577,
                                                 1569, 1562, 1554, 1546, 1538,
                                                 1530, 1523, 1515, 1507, 1499,
                                                 1492, 1484, 1476, 1468, 1461,
                                                 1453, 1445, 1438, 1430, 1422,
                                                 1415, 1407, 1399, 1392, 1384,
                                                 1377, 1369, 1361, 1354, 1346,
                                                 1339, 1331, 1324, 1316, 1309,
                                                 1301, 1294, 1286, 1279, 1271,
                                                 1264, 1256, 1249, 1242, 1234,
                                                 1227, 1219, 1212, 1205, 1197,
                                                 1190, 1183, 1176, 1168, 1161,
                                                 1154, 1147, 1139, 1132, 1125,
                                                 1118, 1111, 1103, 1096, 1089,
                                                 1082, 1075, 1068, 1061, 1054,
                                                 1047, 1040, 1033, 1026, 1019,
                                                 1012, 1005, 998, 991, 984, 977,
                                                 971, 964, 957, 950, 943, 937,
                                                 930, 923, 916, 910, 903, 896,
                                                 890, 883, 876, 870, 863, 857,
                                                 850, 844, 837, 831, 824, 818,
                                                 811, 805, 799, 792, 786, 780,
                                                 773, 767, 761, 754, 748, 742,
                                                 736, 730, 724, 717, 711, 705,
                                                 699, 693, 687, 681, 675, 669,
                                                 663, 657, 651, 646, 640, 634,
                                                 628, 622, 617, 611, 605, 599,
                                                 594, 588, 582, 577, 571, 566,
                                                 560, 555, 549, 544, 538, 533,
                                                 527, 522, 517, 511, 506, 501,
                                                 495, 490, 485, 480, 475, 469,
                                                 464, 459, 454, 449, 444, 439,
                                                 434, 429, 424, 419, 415, 410,
                                                 405, 400, 395, 391, 386, 381,
                                                 377, 372, 367, 363, 358, 354,
                                                 349, 345, 340, 336, 331, 327,
                                                 323, 318, 314, 310, 306, 301,
                                                 297, 293, 289, 285, 281, 277,
                                                 273, 269, 265, 261, 257, 253,
                                                 249, 245, 241, 238, 234, 230,
                                                 226, 223, 219, 215, 212, 208,
                                                 205, 201, 198, 194, 191, 188,
                                                 184, 181, 178, 174, 171, 168,
                                                 165, 162, 158, 155, 152, 149,
                                                 146, 143, 140, 137, 135, 132,
                                                 129, 126, 123, 121, 118, 115,
                                                 113, 110, 107, 105, 102, 100,
                                                 97, 95, 92, 90, 88, 85, 83, 81,
                                                 79, 76, 74, 72, 70, 68, 66, 64,
                                                 62, 60, 58, 56, 54, 52, 51, 49,
                                                 47, 45, 44, 42, 40, 39, 37, 36,
                                                 34, 33, 31, 30, 29, 27, 26, 25,
                                                 23, 22, 21, 20, 19, 18, 17, 16,
                                                 15, 14, 13, 12, 11, 10, 9, 9,
                                                 8, 7, 6, 6, 5, 5, 4, 4, 3, 3,
                                                 2, 2, 1, 1, 1, 1, 0, 0, 0, 0,
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                 0, 1, 1, 1, 1, 2, 2, 3, 3, 4,
                                                 4, 5, 5, 6, 6, 7, 8, 9, 9, 10,
                                                 11, 12, 13, 14, 15, 16, 17, 18,
                                                 19, 20, 21, 22, 23, 25, 26, 27,
                                                 29, 30, 31, 33, 34, 36, 37, 39,
                                                 40, 42, 44, 45, 47, 49, 51, 52,
                                                 54, 56, 58, 60, 62, 64, 66, 68,
                                                 70, 72, 74, 76, 79, 81, 83, 85,
                                                 88, 90, 92, 95, 97, 100, 102,
                                                 105, 107, 110, 113, 115, 118,
                                                 121, 123, 126, 129, 132, 135,
                                                 137, 140, 143, 146, 149, 152,
                                                 155, 158, 162, 165, 168, 171,
                                                 174, 178, 181, 184, 188, 191,
                                                 194, 198, 201, 205, 208, 212,
                                                 215, 219, 223, 226, 230, 234,
                                                 238, 241, 245, 249, 253, 257,
                                                 261, 265, 269, 273, 277, 281,
                                                 285, 289, 293, 297, 301, 306,
                                                 310, 314, 318, 323, 327, 331,
                                                 336, 340, 345, 349, 354, 358,
                                                 363, 367, 372, 377, 381, 386,
                                                 391, 395, 400, 405, 410, 415,
                                                 419, 424, 429, 434, 439, 444,
                                                 449, 454, 459, 464, 469, 475,
                                                 480, 485, 490, 495, 501, 506,
                                                 511, 517, 522, 527, 533, 538,
                                                 544, 549, 555, 560, 566, 571,
                                                 577, 582, 588, 594, 599, 605,
                                                 611, 617, 622, 628, 634, 640,
                                                 646, 651, 657, 663, 669, 675,
                                                 681, 687, 693, 699, 705, 711,
                                                 717, 724, 730, 736, 742, 748,
                                                 754, 761, 767, 773, 780, 786,
                                                 792, 799, 805, 811, 818, 824,
                                                 831, 837, 844, 850, 857, 863,
                                                 870, 876, 883, 890, 896, 903,
                                                 910, 916, 923, 930, 937, 943,
                                                 950, 957, 964, 971, 977, 984,
                                                 991, 998, 1005, 1012, 1019,
                                                 1026, 1033, 1040, 1047, 1054,
                                                 1061, 1068, 1075, 1082, 1089,
                                                 1096, 1103, 1111, 1118, 1125,
                                                 1132, 1139, 1147, 1154, 1161,
                                                 1168, 1176, 1183, 1190, 1197,
                                                 1205, 1212, 1219, 1227, 1234,
                                                 1242, 1249, 1256, 1264, 1271,
                                                 1279, 1286, 1294, 1301, 1309,
                                                 1316, 1324, 1331, 1339, 1346,
                                                 1354, 1361, 1369, 1377, 1384,
                                                 1392, 1399, 1407, 1415, 1422,
                                                 1430, 1438, 1445, 1453, 1461,
                                                 1468, 1476, 1484, 1492, 1499,
                                                 1507, 1515, 1523, 1530, 1538,
                                                 1546, 1554, 1562, 1569, 1577,
                                                 1585, 1593, 1601, 1609, 1616,
                                                 1624, 1632, 1640, 1648, 1656,
                                                 1664, 1672, 1680, 1687, 1695,
                                                 1703, 1711, 1719, 1727, 1735,
                                                 1743, 1751, 1759, 1767, 1775,
                                                 1783, 1791, 1799, 1807, 1815,
                                                 1823, 1831, 1839, 1847, 1855,
                                                 1863, 1871, 1879, 1887, 1895,
                                                 1903, 1911, 1919, 1927, 1935,
                                                 1943, 1951, 1959, 1967, 1975,
                                                 1983, 1991, 1999, 2007, 2015,
                                                 2023, 2031, 2039 };
    time = lookup[time];
    return time;
}

