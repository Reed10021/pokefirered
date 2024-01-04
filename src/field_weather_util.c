#include "global.h"
#include "field_weather.h"
#include "overworld.h"
#include "task.h"
#include "field_weather_effects.h"
#include "constants/weather.h"

static u8 TranslateWeatherNum(u8 weather);
static void UpdateRainCounter(u8 newWeather, u8 oldWeather);

void SetSavedWeather(u32 weather)
{
    u8 oldWeather = gSaveBlock1Ptr->weather;
    gSaveBlock1Ptr->weather = TranslateWeatherNum(weather);
    UpdateRainCounter(gSaveBlock1Ptr->weather, oldWeather);
}

u8 GetSav1Weather(void)
{
    return gSaveBlock1Ptr->weather;
}

void SetSavedWeatherFromCurrMapHeader(void)
{
    u8 oldWeather = gSaveBlock1Ptr->weather;
    gSaveBlock1Ptr->weather = TranslateWeatherNum(gMapHeader.weather);
    UpdateRainCounter(gSaveBlock1Ptr->weather, oldWeather);
}

void SetWeather(u32 weather)
{
    SetSavedWeather(weather);
    SetNextWeather(GetSav1Weather());
}

void SetWeather_Unused(u32 weather)
{
    SetSavedWeather(weather);
    SetCurrentAndNextWeather(GetSav1Weather());
}

void DoCurrentWeather(void)
{
    u8 weather = GetSav1Weather();

    if (weather == WEATHER_ABNORMAL)
    {
        if (!FuncIsActiveTask(Task_DoAbnormalWeather))
            CreateAbnormalWeatherTask();
        weather = gCurrentAbnormalWeather;
    }
    else
    {
        if (FuncIsActiveTask(Task_DoAbnormalWeather))
            DestroyTask(FindTaskIdByFunc(Task_DoAbnormalWeather));
        gCurrentAbnormalWeather = WEATHER_DOWNPOUR;
    }

    SetNextWeather(weather);
}

void ResumePausedWeather(void)
{
    u8 weather = GetSav1Weather();

    if (weather == WEATHER_ABNORMAL)
    {
        if (!FuncIsActiveTask(Task_DoAbnormalWeather))
            CreateAbnormalWeatherTask();
        weather = gCurrentAbnormalWeather;
    }
    else
    {
        if (FuncIsActiveTask(Task_DoAbnormalWeather))
            DestroyTask(FindTaskIdByFunc(Task_DoAbnormalWeather));
        gCurrentAbnormalWeather = WEATHER_DOWNPOUR;
    }

    SetCurrentAndNextWeather(weather);
}

static const u8 sWeatherCycleRoute1_2[] = // and Pallet Town, Viridian
{
    WEATHER_RAIN,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_SNOW,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
};

static const u8 sWeatherCycleViridian_Forest[] = 
{
    WEATHER_RAIN,
    WEATHER_SHADE,
    WEATHER_SHADE,
    WEATHER_SNOW,
    WEATHER_SHADE,
    WEATHER_SHADE,
};

static const u8 sWeatherCycleRoute22_23[] =
{
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_SANDSTORM,
    WEATHER_SUNNY,
    WEATHER_SNOW,
    WEATHER_SUNNY,
    WEATHER_DROUGHT,
};
static const u8 sWeatherCycleRoute3_4[] = // and Pewter
{
    WEATHER_SUNNY,
    WEATHER_SANDSTORM,
    WEATHER_RAIN,
    WEATHER_DROUGHT,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
};

static const u8 sWeatherCycleRoute24_25[] =
{
    WEATHER_SUNNY,
    WEATHER_RAIN,
    WEATHER_DROUGHT,
    WEATHER_SUNNY,
    WEATHER_SNOW,
    WEATHER_SNOW,
};

static const u8 sWeatherCycleCerulean[] =
{
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_DROUGHT,
    WEATHER_SNOW,
    WEATHER_SUNNY,
};

static const u8 sWeatherCycleRoute9_10[] =
{
    WEATHER_DROUGHT,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_RAIN,
    WEATHER_SUNNY,
    WEATHER_SANDSTORM,
};

static const u8 sWeatherCycleRoute11[] = // and Vermillion
{
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_RAIN,
    WEATHER_DROUGHT,
    WEATHER_DOWNPOUR,
};

static const u8 sWeatherCycleRoute5_6_7_8[] = // and Saffron City
{
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_RAIN,
    WEATHER_SUNNY,
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_DROUGHT,
};

static const u8 sWeatherCycleLavender[] =
{
    WEATHER_SUNNY,
    WEATHER_ABNORMAL,
    WEATHER_SUNNY,
    WEATHER_RAIN,
    WEATHER_SUNNY,
    WEATHER_ABNORMAL,
};

static const u8 sWeatherCycleRoute12_13_14[] =
{
    WEATHER_SNOW,
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_DROUGHT,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_DROUGHT,
};

static const u8 sWeatherCycleRoute15_Safari[] = // and Fuchsia
{
    WEATHER_DROUGHT,
    WEATHER_DOWNPOUR,
    WEATHER_SUNNY,
    WEATHER_DROUGHT,
    WEATHER_RAIN,
    WEATHER_SUNNY,
};

static const u8 sWeatherCycleRoute19_20[] =
{
    WEATHER_SUNNY,
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_SNOW,
    WEATHER_SUNNY,
    WEATHER_SNOW,
    WEATHER_DOWNPOUR,
};

static const u8 sWeatherCycleRoute21[] = // N and S, and Cinnabar
{
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_DROUGHT,
    WEATHER_SUNNY,
    WEATHER_SNOW,
    WEATHER_SUNNY,
    WEATHER_RAIN,
};

static const u8 sWeatherCycleRoute16_17_18[] = // and Celadon
{
    WEATHER_RAIN,
    WEATHER_SUNNY,
    WEATHER_SUNNY,
    WEATHER_RAIN_THUNDERSTORM,
    WEATHER_DOWNPOUR,
    WEATHER_DROUGHT,
};

static u8 TranslateWeatherNum(u8 weather)
{
    switch (weather)
    {
    case WEATHER_NONE:               return WEATHER_NONE;
    case WEATHER_SUNNY_CLOUDS:       return WEATHER_SUNNY_CLOUDS;
    case WEATHER_SUNNY:              return WEATHER_SUNNY;
    case WEATHER_RAIN:               return WEATHER_RAIN;
    case WEATHER_SNOW:               return WEATHER_SNOW;
    case WEATHER_RAIN_THUNDERSTORM:  return WEATHER_RAIN_THUNDERSTORM;
    case WEATHER_FOG_HORIZONTAL:     return WEATHER_FOG_HORIZONTAL;
    case WEATHER_VOLCANIC_ASH:       return WEATHER_VOLCANIC_ASH;
    case WEATHER_SANDSTORM:          return WEATHER_SANDSTORM;
    case WEATHER_FOG_DIAGONAL:       return WEATHER_FOG_DIAGONAL;
    case WEATHER_UNDERWATER:         return WEATHER_UNDERWATER;
    case WEATHER_SHADE:              return WEATHER_SHADE;
    case WEATHER_DROUGHT:            return WEATHER_DROUGHT;
    case WEATHER_DOWNPOUR:           return WEATHER_DOWNPOUR;
    case WEATHER_UNDERWATER_BUBBLES: return WEATHER_UNDERWATER_BUBBLES;
    case WEATHER_ABNORMAL:           return WEATHER_ABNORMAL;
    case WEATHER_ROUTE1_2_CYCLE:       return sWeatherCycleRoute1_2[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE3_4_CYCLE:       return sWeatherCycleRoute3_4[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE5_6_7_8_CYCLE:       return sWeatherCycleRoute5_6_7_8[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE9_10_CYCLE:       return sWeatherCycleRoute9_10[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE11_CYCLE:       return sWeatherCycleRoute11[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE12_13_14_CYCLE:       return sWeatherCycleRoute12_13_14[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE15_SAFARI_CYCLE:       return sWeatherCycleRoute15_Safari[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE16_17_18_CYCLE:       return sWeatherCycleRoute16_17_18[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE19_20_CYCLE:       return sWeatherCycleRoute19_20[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE21_CYCLE:       return sWeatherCycleRoute21[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE22_23_CYCLE:       return sWeatherCycleRoute22_23[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_ROUTE24_25_CYCLE:       return sWeatherCycleRoute24_25[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_CERULEAN_CYCLE:       return sWeatherCycleCerulean[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_LAVENDER_CYCLE:       return sWeatherCycleLavender[gSaveBlock1Ptr->weatherCycleStage];
    case WEATHER_VIRIDIAN_FOREST_CYCLE:       return sWeatherCycleViridian_Forest[gSaveBlock1Ptr->weatherCycleStage];
    default:                         return WEATHER_NONE;
    }
}

void UpdateWeatherPerDay(u16 increment)
{
    u16 weatherStage = gSaveBlock1Ptr->weatherCycleStage + increment;
    weatherStage %= 6; // Weather cycle shifts by 1 per week. i.e Monday's weather will be Sunday's weather next week.
    gSaveBlock1Ptr->weatherCycleStage = weatherStage;
}


static void UpdateRainCounter(u8 newWeather, u8 oldWeather)
{
    if (newWeather != oldWeather
        && (newWeather == WEATHER_RAIN || newWeather == WEATHER_RAIN_THUNDERSTORM || newWeather == WEATHER_DOWNPOUR))
        IncrementGameStat(GAME_STAT_GOT_RAINED_ON);
}
