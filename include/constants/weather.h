#ifndef GUARD_CONSTANTS_WEATHER_H
#define GUARD_CONSTANTS_WEATHER_H

#define WEATHER_NONE               0
#define WEATHER_SUNNY_CLOUDS       1   // unused
#define WEATHER_SUNNY              2
#define WEATHER_RAIN               3   // unused
#define WEATHER_SNOW               4   // unused
#define WEATHER_RAIN_THUNDERSTORM  5   // unused
#define WEATHER_FOG_HORIZONTAL     6
#define WEATHER_VOLCANIC_ASH       7   // unused
#define WEATHER_SANDSTORM          8   // unused
#define WEATHER_FOG_DIAGONAL       9   // unused
#define WEATHER_UNDERWATER         10  // unused
#define WEATHER_SHADE              11
#define WEATHER_DROUGHT            12  // unused and broken in overworld
#define WEATHER_DOWNPOUR           13  // unused
#define WEATHER_UNDERWATER_BUBBLES 14  // unused
#define WEATHER_ABNORMAL           15  // unused
#define WEATHER_ROUTE119_CYCLE     20  // unused
#define WEATHER_ROUTE123_CYCLE     21  // unused

#define WEATHER_ROUTE102_104_CYCLE      22  // unused
#define WEATHER_ROUTE105_CYCLE          23  // unused
#define WEATHER_ROUTE106_107_CYCLE      24  // unused
#define WEATHER_ROUTE108_109_CYCLE      25  // unused
#define WEATHER_ROUTE103_110_CYCLE      26  // unused
#define WEATHER_ROUTE111_112_CYCLE      27  // unused
#define WEATHER_ROUTE114_115_CYCLE      28  // unused
#define WEATHER_ROUTE116_117_CYCLE      29  // unused
#define WEATHER_ROUTE120_CYCLE          30  // unused
#define WEATHER_ROUTE121_122_CYCLE      31  // unused
#define WEATHER_ROUTE124_125_CYCLE      32  // unused
#define WEATHER_ROUTE126_127_128_CYCLE  33  // unused
#define WEATHER_ROUTE129_130_131_CYCLE  34  // unused
#define WEATHER_ROUTE132_133_134_CYCLE  35  // unused
#define WEATHER_SLATEPORT_CYCLE         36  // unused
#define WEATHER_PACIFIDLOG_CYCLE        37  // unused
#define WEATHER_LILYCOVE_CYCLE          38  // unused
#define WEATHER_PETALBURGWOODS_CYCLE    39  // unused
#define WEATHER_TIMELESS_FOREST         40  // unused
#define WEATHER_ROUTE1_2_CYCLE          41
#define WEATHER_ROUTE3_4_CYCLE          42
#define WEATHER_ROUTE5_6_7_8_CYCLE      43
#define WEATHER_ROUTE9_10_CYCLE         44
#define WEATHER_ROUTE11_CYCLE           45
#define WEATHER_ROUTE12_13_14_CYCLE     46
#define WEATHER_ROUTE15_SAFARI_CYCLE    47
#define WEATHER_ROUTE16_17_18_CYCLE     48
#define WEATHER_ROUTE19_20_CYCLE        49
#define WEATHER_ROUTE21_CYCLE           50
#define WEATHER_ROUTE22_23_CYCLE        51
#define WEATHER_ROUTE24_25_CYCLE        52
#define WEATHER_CERULEAN_CYCLE          53
#define WEATHER_LAVENDER_CYCLE          54
#define WEATHER_VIRIDIAN_FOREST_CYCLE   55

// These are used in maps' coord_weather_event entries.
// They are not a one-to-one mapping with the engine's
// internal weather constants above.
#define COORD_EVENT_WEATHER_SUNNY_CLOUDS        1
#define COORD_EVENT_WEATHER_SUNNY               2
#define COORD_EVENT_WEATHER_RAIN                3
#define COORD_EVENT_WEATHER_SNOW                4
#define COORD_EVENT_WEATHER_RAIN_THUNDERSTORM   5
#define COORD_EVENT_WEATHER_FOG_HORIZONTAL      6
#define COORD_EVENT_WEATHER_FOG_DIAGONAL        7
#define COORD_EVENT_WEATHER_VOLCANIC_ASH        8
#define COORD_EVENT_WEATHER_SANDSTORM           9
#define COORD_EVENT_WEATHER_SHADE               10
#define COORD_EVENT_WEATHER_DROUGHT             11
#define COORD_EVENT_WEATHER_ROUTE119_CYCLE      20
#define COORD_EVENT_WEATHER_ROUTE123_CYCLE      21

#endif  // GUARD_CONSTANTS_WEATHER_H
