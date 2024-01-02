#include "global.h"
#include "time_events.h"
#include "event_data.h"
#include "field_weather.h"
#include "pokemon.h"
#include "random.h"
#include "overworld.h"
#include "rtc.h"
#include "script.h"
#include "task.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/region_map_sections.h"

static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
    u16 locationMap;
    // Include mapsec so we can refer to the map name properly.
} sPokeOutbreakSpeciesList[] = {
    {
        .species = SPECIES_TOGEPI,
        .moves = {MOVE_ANCIENT_POWER, MOVE_TRI_ATTACK, MOVE_FOLLOW_ME, MOVE_HELPING_HAND},
        .level = 8,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_TOGETIC,
        .moves = {MOVE_ICE_BEAM, MOVE_TRI_ATTACK, MOVE_TAIL_GLOW, MOVE_THUNDER},
        .level = 15,
        .location = MAP_NUM(ROUTE22),
        .locationMap = MAPSEC_ROUTE_22
    },
    {
        .species = SPECIES_FEEBAS,
        .moves = {MOVE_TAIL_GLOW, MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_THUNDER},
        .level = 20,
        .location = MAP_NUM(ROUTE19),
        .locationMap = MAPSEC_ROUTE_19
    },
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BIDE, MOVE_HARDEN, MOVE_LEECH_SEED},
        .level = 3,
        .location = MAP_NUM(ROUTE2),
        .locationMap = MAPSEC_ROUTE_2
    },
    {
        .species = SPECIES_FARFETCHD,
        .moves = {MOVE_SLASH, MOVE_FOCUS_ENERGY, MOVE_AIR_CUTTER, MOVE_DRAGON_DANCE},
        .level = 10,
        .location = MAP_NUM(ROUTE24),
        .locationMap = MAPSEC_ROUTE_24
    },
    {
        .species = SPECIES_POOCHYENA,
        .moves = {MOVE_DRAGON_DANCE, MOVE_CRUNCH, MOVE_SHADOW_BALL, MOVE_HYPER_FANG},
        .level = 21,
        .location = MAP_NUM(ROUTE2),
        .locationMap = MAPSEC_ROUTE_2
    },
    {
        .species = SPECIES_AIPOM,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FALSE_SWIPE, MOVE_YAWN, MOVE_THRASH},
        .level = 17,
        .location = MAP_NUM(ROUTE5),
        .locationMap = MAPSEC_ROUTE_5
    },
    {
        .species = SPECIES_NUZLEAF,
        .moves = {MOVE_HARDEN, MOVE_GROWTH, MOVE_NATURE_POWER, MOVE_LEECH_SEED},
        .level = 15,
        .location = MAP_NUM(ROUTE11),
        .locationMap = MAPSEC_ROUTE_11
    },
    {
        .species = SPECIES_STEELIX,
        .moves = {MOVE_THRASH, MOVE_IRON_TAIL, MOVE_EARTHQUAKE, MOVE_ROCK_BLAST},
        .level = 21,
        .location = MAP_NUM(ROUTE24),
        .locationMap = MAPSEC_ROUTE_24
    },
    {
        .species = SPECIES_SLOWPOKE,
        .moves = {MOVE_PSYCHIC, MOVE_TAIL_GLOW, MOVE_AMNESIA, MOVE_FUTURE_SIGHT},
        .level = 20,
        .location = MAP_NUM(ROUTE12),
        .locationMap = MAPSEC_ROUTE_12
    },
    {
        .species = SPECIES_ANORITH,
        .moves = {MOVE_SWORDS_DANCE, MOVE_FURY_CUTTER, MOVE_ROCK_SLIDE, MOVE_EARTHQUAKE},
        .level = 26,
        .location = MAP_NUM(ROUTE13),
        .locationMap = MAPSEC_ROUTE_13
    },
    {
        .species = SPECIES_ZIGZAGOON,
        .moves = {MOVE_BELLY_DRUM, MOVE_SLASH, MOVE_REST, MOVE_SLEEP_TALK},
        .level = 13,
        .location = MAP_NUM(ROUTE2),
        .locationMap = MAPSEC_ROUTE_2
    },
    {
        .species = SPECIES_CASTFORM,
        .moves = {MOVE_WEATHER_BALL, MOVE_SWORDS_DANCE, MOVE_SANDSTORM, MOVE_EARTHQUAKE},
        .level = 30,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_RALTS,
        .moves = {MOVE_TELEPORT, MOVE_PSYCHIC, MOVE_FIRE_PUNCH, MOVE_ICE_PUNCH},
        .level = 14,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_SKITTY,
        .moves = {MOVE_GROWL, MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_ATTRACT},
        .level = 8,
        .location = MAP_NUM(ROUTE2),
        .locationMap = MAPSEC_ROUTE_2
    },
    {
        .species = SPECIES_TROPIUS,
        .moves = {MOVE_OUTRAGE, MOVE_DRAGON_DANCE, MOVE_AEROBLAST, MOVE_LEAF_BLADE},
        .level = 17,
        .location = MAP_NUM(ROUTE6),
        .locationMap = MAPSEC_ROUTE_6
    },
    {
        .species = SPECIES_LILEEP,
        .moves = {MOVE_DRAGON_DANCE, MOVE_GIGA_DRAIN, MOVE_ROCK_SLIDE, MOVE_CRUNCH},
        .level = 31,
        .location = MAP_NUM(ROUTE13),
        .locationMap = MAPSEC_ROUTE_13
    },
    {
        .species = SPECIES_MEDITITE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_PSYCHIC, MOVE_CROSS_CHOP, MOVE_LEAF_BLADE},
        .level = 13,
        .location = MAP_NUM(ROUTE2),
        .locationMap = MAPSEC_ROUTE_2
    },
    {
        .species = SPECIES_SWABLU,
        .moves = {MOVE_DRAGON_CLAW, MOVE_THUNDERBOLT, MOVE_FLY, MOVE_DRAGON_DANCE},
        .level = 12,
        .location = MAP_NUM(ROUTE22),
        .locationMap = MAPSEC_ROUTE_22
    },
    {
        .species = SPECIES_FARFETCHD,
        .moves = {MOVE_SLASH, MOVE_FOCUS_ENERGY, MOVE_AIR_CUTTER, MOVE_DRAGON_DANCE},
        .level = 15,
        .location = MAP_NUM(ROUTE10),
        .locationMap = MAPSEC_ROUTE_10
    },
    {
        .species = SPECIES_TRAPINCH,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FURY_CUTTER, MOVE_EARTHQUAKE, MOVE_METEOR_MASH},
        .level = 22,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_BARBOACH,
        .moves = {MOVE_BELLY_DRUM, MOVE_TAIL_GLOW, MOVE_EARTHQUAKE, MOVE_SURF},
        .level = 20,
        .location = MAP_NUM(ROUTE12),
        .locationMap = MAPSEC_ROUTE_12
    },
    {
        .species = SPECIES_SLAKOTH,
        .moves = {MOVE_YAWN, MOVE_BELLY_DRUM, MOVE_THRASH, MOVE_METEOR_MASH},
        .level = 14,
        .location = MAP_NUM(ROUTE6),
        .locationMap = MAPSEC_ROUTE_6
    },
    {
        .species = SPECIES_WURMPLE,
        .moves = {MOVE_FURY_CUTTER, MOVE_DRAGON_DANCE, MOVE_SLUDGE_BOMB, MOVE_SPORE},
        .level = 14,
        .location = MAP_NUM(ROUTE10),
        .locationMap = MAPSEC_ROUTE_10
    },
    {
        .species = SPECIES_CASTFORM,
        .moves = {MOVE_WEATHER_BALL, MOVE_TAIL_GLOW, MOVE_RAIN_DANCE, MOVE_ICE_BALL},
        .level = 30,
        .location = MAP_NUM(ROUTE6),
        .locationMap = MAPSEC_ROUTE_6
    },
    {
        .species = SPECIES_LOTAD,
        .moves = {MOVE_GIGA_DRAIN, MOVE_TAIL_GLOW, MOVE_SURF, MOVE_SPORE},
        .level = 17,
        .location = MAP_NUM(ROUTE8),
        .locationMap = MAPSEC_ROUTE_8
    },
    {
        .species = SPECIES_NUMEL,
        .moves = {MOVE_GROWTH, MOVE_EARTHQUAKE, MOVE_FLAMETHROWER, MOVE_GIGA_DRAIN},
        .level = 19,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_PINSIR,
        .moves = {MOVE_SWORDS_DANCE, MOVE_FURY_CUTTER, MOVE_FALSE_SWIPE, MOVE_CROSS_CHOP},
        .level = 16,
        .location = MAP_NUM(ROUTE5),
        .locationMap = MAPSEC_ROUTE_5
    },
    {
        .species = SPECIES_SCYTHER,
        .moves = {MOVE_DRAGON_DANCE, MOVE_YAWN, MOVE_CROSS_CHOP, MOVE_DRILL_PECK},
        .level = 20,
        .location = MAP_NUM(ROUTE13),
        .locationMap = MAPSEC_ROUTE_13
    },
    {
        .species = SPECIES_HOUNDOUR,
        .moves = {MOVE_TAIL_GLOW, MOVE_YAWN, MOVE_HEAT_WAVE, MOVE_DRAGON_BREATH},
        .level = 14,
        .location = MAP_NUM(ROUTE4),
        .locationMap = MAPSEC_ROUTE_4
    },
    {
        .species = SPECIES_NOSEPASS,
        .moves = {MOVE_METEOR_MASH, MOVE_SWORDS_DANCE, MOVE_ROCK_SLIDE, MOVE_HYPNOSIS},
        .level = 15,
        .location = MAP_NUM(ROUTE10),
        .locationMap = MAPSEC_ROUTE_10
    },
    {
        .species = SPECIES_LARVITAR,
        .moves = {MOVE_DRAGON_DANCE, MOVE_EARTHQUAKE, MOVE_THIEF, MOVE_ROCK_SLIDE},
        .level = 10,
        .location = MAP_NUM(ROUTE4),
        .locationMap = MAPSEC_ROUTE_4
    },
    {
        .species = SPECIES_BLISSEY,
        .moves = {MOVE_AMNESIA, MOVE_SWEET_KISS, MOVE_WISH, MOVE_HYDRO_PUMP},
        .level = 50,
        .location = MAP_NUM(ROUTE15),
        .locationMap = MAPSEC_ROUTE_15
    },
    {
        .species = SPECIES_MILTANK,
        .moves = {MOVE_CURSE, MOVE_ROLLOUT, MOVE_THRASH, MOVE_SHADOW_BALL},
        .level = 50,
        .location = MAP_NUM(ROUTE15),
        .locationMap = MAPSEC_ROUTE_15
    },
    {
        .species = SPECIES_MAGBY,
        .moves = {MOVE_THUNDER_PUNCH, MOVE_ICE_PUNCH, MOVE_HEAT_WAVE, MOVE_TAIL_GLOW},
        .level = 19,
        .location = MAP_NUM(ROUTE5),
        .locationMap = MAPSEC_ROUTE_5
    },
    {
        .species = SPECIES_CASTFORM,
        .moves = {MOVE_WEATHER_BALL, MOVE_TAIL_GLOW, MOVE_SUNNY_DAY, MOVE_SOLAR_BEAM},
        .level = 30,
        .location = MAP_NUM(ROUTE7),
        .locationMap = MAPSEC_ROUTE_7
    },
    {
        .species = SPECIES_AIPOM,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FALSE_SWIPE, MOVE_YAWN, MOVE_THRASH},
        .level = 14,
        .location = MAP_NUM(ROUTE3),
        .locationMap = MAPSEC_ROUTE_3
    },
    {
        .species = SPECIES_ELEKID,
        .moves = {MOVE_ICE_PUNCH, MOVE_FIRE_PUNCH, MOVE_VOLT_TACKLE, MOVE_TAIL_GLOW},
        .level = 21,
        .location = MAP_NUM(ROUTE5),
        .locationMap = MAPSEC_ROUTE_5
    },
    {
        .species = SPECIES_TYROGUE,
        .moves = {MOVE_SWORDS_DANCE, MOVE_SHADOW_BALL, MOVE_CROSS_CHOP, MOVE_METEOR_MASH},
        .level = 20,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_LARVITAR,
        .moves = {MOVE_DRAGON_DANCE, MOVE_METEOR_MASH, MOVE_THIEF, MOVE_ROCK_SLIDE},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_CRAWDAUNT,
        .moves = {MOVE_DRAGON_DANCE, MOVE_SURF, MOVE_CRUNCH, MOVE_CROSS_CHOP},
        .level = 30,
        .location = MAP_NUM(ROUTE19),
        .locationMap = MAPSEC_ROUTE_19
    },
    {
        .species = SPECIES_BELDUM,
        .moves = {MOVE_ICE_PUNCH, MOVE_THUNDER_PUNCH, MOVE_IRON_DEFENSE, MOVE_DRAGON_DANCE},
        .level = 21,
        .location = MAP_NUM(ROUTE10),
        .locationMap = MAPSEC_ROUTE_10
    },
    {   // Hoenn Starter outbreaks
        .species = SPECIES_TREECKO,
        .moves = {MOVE_TAIL_GLOW, MOVE_ICE_PUNCH, MOVE_YAWN, MOVE_GIGA_DRAIN},
        .level = 17,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_TORCHIC,
        .moves = {MOVE_DRAGON_DANCE, MOVE_THUNDER_PUNCH, MOVE_CROSS_CHOP, MOVE_HEAT_WAVE},
        .level = 17,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_MUDKIP,
        .moves = {MOVE_SWORDS_DANCE, MOVE_FURY_CUTTER, MOVE_SLUDGE_BOMB, MOVE_EARTHQUAKE},
        .level = 17,
        .location = MAP_NUM(ROUTE9),
        .locationMap = MAPSEC_ROUTE_9
    },
    {
        .species = SPECIES_TREECKO,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FRENZY_PLANT, MOVE_THUNDER_PUNCH, MOVE_DRAGON_CLAW},
        .level = 25,
        .location = MAP_NUM(ROUTE15),
        .locationMap = MAPSEC_ROUTE_15
    },
    {
        .species = SPECIES_TORCHIC,
        .moves = {MOVE_DRAGON_DANCE, MOVE_METEOR_MASH, MOVE_CROSS_CHOP, MOVE_BLAST_BURN},
        .level = 23,
        .location = MAP_NUM(ROUTE14),
        .locationMap = MAPSEC_ROUTE_14
    },
    {
        .species = SPECIES_MUDKIP,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FURY_CUTTER, MOVE_HYDRO_CANNON, MOVE_EARTHQUAKE},
        .level = 24,
        .location = MAP_NUM(ROUTE13),
        .locationMap = MAPSEC_ROUTE_13
    },
    {
        .species = SPECIES_CHINCHOU,
        .moves = {MOVE_DRAGON_DANCE, MOVE_SURF, MOVE_OUTRAGE, MOVE_PSYCHIC},
        .level = 25,
        .location = MAP_NUM(ROUTE12),
        .locationMap = MAPSEC_ROUTE_12
    },
    {
        .species = SPECIES_LAPRAS,
        .moves = {MOVE_HAIL, MOVE_ICE_BEAM, MOVE_TAIL_GLOW, MOVE_YAWN},
        .level = 30,
        .location = MAP_NUM(ROUTE20),
        .locationMap = MAPSEC_ROUTE_20
    },
    {
        .species = SPECIES_GOREBYSS,
        .moves = {MOVE_THUNDER_WAVE, MOVE_TAIL_GLOW, MOVE_MUDDY_WATER, MOVE_SHEER_COLD},
        .level = 40,
        .location = MAP_NUM(ROUTE20),
        .locationMap = MAPSEC_ROUTE_20
    },
    {
        .species = SPECIES_LAPRAS,
        .moves = {MOVE_RAIN_DANCE, MOVE_HYDRO_CANNON, MOVE_TAIL_GLOW, MOVE_YAWN},
        .level = 30,
        .location = MAP_NUM(ROUTE19),
        .locationMap = MAPSEC_ROUTE_19
    },
    {
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_RAIN_DANCE, MOVE_DRAGON_DANCE, MOVE_DRILL_PECK, MOVE_THRASH},
        .level = 35,
        .location = MAP_NUM(ROUTE21_SOUTH),
        .locationMap = MAPSEC_ROUTE_21
    },
    {
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FURY_CUTTER, MOVE_DRILL_PECK, MOVE_THRASH},
        .level = 38,
        .location = MAP_NUM(ROUTE20),
        .locationMap = MAPSEC_ROUTE_20
    },
    {
        .species = SPECIES_CASTFORM,
        .moves = {MOVE_WEATHER_BALL, MOVE_TAIL_GLOW, MOVE_HAIL, MOVE_PSYCHIC},
        .level = 30,
        .location = MAP_NUM(ROUTE8),
        .locationMap = MAPSEC_ROUTE_8
    },
    {
        .species = SPECIES_SNORLAX,
        .moves = {MOVE_BELLY_DRUM, MOVE_REST, MOVE_THRASH, MOVE_EARTHQUAKE},
        .level = 35,
        .location = MAP_NUM(ROUTE11),
        .locationMap = MAPSEC_ROUTE_11
    },
    {
        .species = SPECIES_HUNTAIL,
        .moves = {MOVE_FURY_CUTTER, MOVE_DRAGON_DANCE, MOVE_HYDRO_CANNON, MOVE_COUNTER},
        .level = 40,
        .location = MAP_NUM(ROUTE21_SOUTH),
        .locationMap = MAPSEC_ROUTE_21
    },
    {
        .species = SPECIES_SNORLAX,
        .moves = {MOVE_BELLY_DRUM, MOVE_BOUNCE, MOVE_THRASH, MOVE_EARTHQUAKE},
        .level = 37,
        .location = MAP_NUM(ROUTE16),
        .locationMap = MAPSEC_ROUTE_16
    },
    {
        .species = SPECIES_PORYGON,
        .moves = {MOVE_BELLY_DRUM, MOVE_SHADOW_BALL, MOVE_TRI_ATTACK, MOVE_CONVERSION_2},
        .level = 20,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_BLISSEY,
        .moves = {MOVE_CALM_MIND, MOVE_SWEET_KISS, MOVE_WISH, MOVE_PSYCHO_BOOST},
        .level = 100,
        .location = MAP_NUM(ROUTE23),
        .locationMap = MAPSEC_ROUTE_23
    },
    {
        .species = SPECIES_MILOTIC,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_THUNDER},
        .level = 60,
        .location = MAP_NUM(ROUTE21_SOUTH),
        .locationMap = MAPSEC_ROUTE_21
    },
    {
        .species = SPECIES_BELDUM,
        .moves = {MOVE_ICE_PUNCH, MOVE_THUNDER_PUNCH, MOVE_IRON_DEFENSE, MOVE_DRAGON_DANCE},
        .level = 8,
        .location = MAP_NUM(ROUTE22),
        .locationMap = MAPSEC_ROUTE_22
    },
    {
        .species = SPECIES_KINGDRA,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_PUMP, MOVE_OUTRAGE, MOVE_THUNDER},
        .level = 45,
        .location = MAP_NUM(ROUTE12),
        .locationMap = MAPSEC_ROUTE_12
    },
    {
        .species = SPECIES_MANTINE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_CANNON, MOVE_BULLET_SEED, MOVE_PSYCHO_BOOST},
        .level = 60,
        .location = MAP_NUM(ROUTE19),
        .locationMap = MAPSEC_ROUTE_19
    },
    {
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_TAIL_GLOW, MOVE_HYDRO_CANNON, MOVE_BELLY_DRUM, MOVE_PSYCHO_BOOST},
        .level = 60,
        .location = MAP_NUM(ROUTE20),
        .locationMap = MAPSEC_ROUTE_20
    },
    {
        .species = SPECIES_SLOWKING,
        .moves = {MOVE_TAIL_GLOW, MOVE_HYDRO_CANNON, MOVE_BELLY_DRUM, MOVE_PSYCHO_BOOST},
        .level = 60,
        .location = MAP_NUM(ROUTE19),
        .locationMap = MAPSEC_ROUTE_19
    },
    {
        .species = SPECIES_POLIWRATH,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_CANNON, MOVE_BELLY_DRUM, MOVE_CROSS_CHOP},
        .level = 35,
        .location = MAP_NUM(ROUTE10),
        .locationMap = MAPSEC_ROUTE_10
    },
    {
        .species = SPECIES_BAGON,
        .moves = {MOVE_DRAGON_DANCE, MOVE_AEROBLAST, MOVE_OUTRAGE, MOVE_SACRED_FIRE},
        .level = 21,
        .location = MAP_NUM(ROUTE4),
        .locationMap = MAPSEC_ROUTE_4
    },
    {
        .species = SPECIES_MASQUERAIN,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_PUMP, MOVE_FLY, MOVE_FURY_CUTTER},
        .level = 50,
        .location = MAP_NUM(ROUTE21_SOUTH),
        .locationMap = MAPSEC_ROUTE_21
    },
    {
        .species = SPECIES_EEVEE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_THIEF, MOVE_SHADOW_BALL, MOVE_ACID_ARMOR},
        .level = 13,
        .location = MAP_NUM(ROUTE4),
        .locationMap = MAPSEC_ROUTE_4
    },
    {
        .species = SPECIES_WAILORD,
        .moves = {MOVE_TAIL_GLOW, MOVE_WATER_SPOUT, MOVE_PSYCHIC, MOVE_CRUNCH},
        .level = 40,
        .location = MAP_NUM(ROUTE20),
        .locationMap = MAPSEC_ROUTE_20
    },
    {
        .species = SPECIES_OMASTAR,
        .moves = {MOVE_DRAGON_DANCE, MOVE_ROCK_SLIDE, MOVE_ICE_BALL, MOVE_HYDRO_CANNON},
        .level = 35,
        .location = MAP_NUM(ROUTE14),
        .locationMap = MAPSEC_ROUTE_14
    },
    {
        .species = SPECIES_KABUTOPS,
        .moves = {MOVE_DRAGON_DANCE, MOVE_ROCK_SLIDE, MOVE_EARTHQUAKE, MOVE_HYDRO_CANNON},
        .level = 35,
        .location = MAP_NUM(ROUTE14),
        .locationMap = MAPSEC_ROUTE_14
    },
    {
        .species = SPECIES_DRATINI,
        .moves = {MOVE_TAIL_GLOW, MOVE_DRAGON_BREATH, MOVE_FLAMETHROWER, MOVE_THUNDERBOLT},
        .level = 20,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_POLITOED,
        .moves = {MOVE_TAIL_GLOW, MOVE_HYDRO_CANNON, MOVE_ICE_BEAM, MOVE_THUNDER_PUNCH},
        .level = 50,
        .location = MAP_NUM(ROUTE17),
        .locationMap = MAPSEC_ROUTE_17
    },
    {
        .species = SPECIES_LARVITAR,
        .moves = {MOVE_SWORDS_DANCE, MOVE_EARTHQUAKE, MOVE_METEOR_MASH, MOVE_FURY_CUTTER},
        .level = 23,
        .location = MAP_NUM(ROUTE16),
        .locationMap = MAPSEC_ROUTE_16
    },
    {
        // Gen 1 starters
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_CURSE, MOVE_CROSS_CHOP, MOVE_HYDRO_CANNON},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_CURSE, MOVE_METEOR_MASH, MOVE_CROSS_CHOP, MOVE_HYDRO_CANNON},
        .level = 25,
        .location = MAP_NUM(ROUTE12),
        .locationMap = MAPSEC_ROUTE_12
    },
    {
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_DRAGON_DANCE, MOVE_DRILL_PECK, MOVE_HEAT_WAVE, MOVE_OUTRAGE},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_DRAGON_DANCE, MOVE_DRILL_PECK, MOVE_BLAST_BURN, MOVE_OUTRAGE},
        .level = 25,
        .location = MAP_NUM(ROUTE6),
        .locationMap = MAPSEC_ROUTE_6
    },
    {
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_DRAGON_DANCE, MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_SPORE},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FRENZY_PLANT, MOVE_SLUDGE_BOMB, MOVE_SPORE},
        .level = 25,
        .location = MAP_NUM(ROUTE11),
        .locationMap = MAPSEC_ROUTE_11
    },
    {
        .species = SPECIES_PELIPPER,
        .moves = {MOVE_BELLY_DRUM, MOVE_FLY, MOVE_SURF, MOVE_EXTREME_SPEED},
        .level = 30,
        .location = MAP_NUM(ROUTE18),
        .locationMap = MAPSEC_ROUTE_18
    },
    {
        // Gen 2 starter(s),
        .species = SPECIES_CHIKORITA,
        .moves = {MOVE_DRAGON_DANCE, MOVE_LEAF_BLADE, MOVE_BONEMERANG, MOVE_SPORE},
        .level = 15,
        .location = MAP_NUM(ROUTE3),
        .locationMap = MAPSEC_ROUTE_3
    },
    {
        .species = SPECIES_CHIKORITA,
        .moves = {MOVE_DRAGON_DANCE, MOVE_PETAL_DANCE, MOVE_MUD_SHOT, MOVE_SPORE},
        .level = 15,
        .location = MAP_NUM(ROUTE14),
        .locationMap = MAPSEC_ROUTE_14
    },
    {
        .species = SPECIES_CYNDAQUIL,
        .moves = {MOVE_TAIL_GLOW, MOVE_HEAT_WAVE, MOVE_ICE_PUNCH, MOVE_HYPNOSIS},
        .level = 15,
        .location = MAP_NUM(ROUTE3),
        .locationMap = MAPSEC_ROUTE_3
    },
    {
        .species = SPECIES_DRATINI,
        .moves = {MOVE_TAIL_GLOW, MOVE_OUTRAGE, MOVE_FLAMETHROWER, MOVE_HYDRO_PUMP},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_CYNDAQUIL,
        .moves = {MOVE_DRAGON_DANCE, MOVE_BLAST_BURN, MOVE_FURY_CUTTER, MOVE_ICE_PUNCH},
        .level = 25,
        .location = MAP_NUM(ROUTE7),
        .locationMap = MAPSEC_ROUTE_7
    },
    {
        .species = SPECIES_TOTODILE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_WATERFALL, MOVE_METEOR_MASH, MOVE_THRASH},
        .level = 15,
        .location = MAP_NUM(ROUTE3),
        .locationMap = MAPSEC_ROUTE_3
    },
    {
        .species = SPECIES_TOTODILE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HYDRO_CANNON, MOVE_FURY_CUTTER, MOVE_METEOR_MASH},
        .level = 25,
        .location = MAP_NUM(ROUTE17),
        .locationMap = MAPSEC_ROUTE_17
    },
    {
        .species = SPECIES_SHARPEDO,
        .moves = {MOVE_TAIL_GLOW, MOVE_HYDRO_PUMP, MOVE_CRUNCH, MOVE_FURY_CUTTER},
        .level = 30,
        .location = MAP_NUM(ROUTE21_SOUTH),
        .locationMap = MAPSEC_ROUTE_21
    },
    {
        .species = SPECIES_SNORUNT,
        .moves = {MOVE_DRAGON_DANCE, MOVE_HAIL, MOVE_BLIZZARD, MOVE_CRUNCH},
        .level = 21,
        .location = MAP_NUM(ROUTE24),
        .locationMap = MAPSEC_ROUTE_24
    },
    {
        .species = SPECIES_WYNAUT,
        .moves = {MOVE_DRAGON_DANCE, MOVE_COUNTER, MOVE_MIRROR_COAT, MOVE_DESTINY_BOND},
        .level = 23,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_DRAGON_DANCE, MOVE_SURF, MOVE_DREAM_EATER, MOVE_HYPNOSIS},
        .level = 35,
        .location = MAP_NUM(ROUTE18),
        .locationMap = MAPSEC_ROUTE_18
    },
    {
        .species = SPECIES_EEVEE,
        .moves = {MOVE_TAIL_GLOW, MOVE_SURF, MOVE_FLAMETHROWER, MOVE_THUNDERBOLT},
        .level = 10,
        .location = MAP_NUM(ROUTE25),
        .locationMap = MAPSEC_ROUTE_25
    },
    {
        .species = SPECIES_CACNEA,
        .moves = {MOVE_DRAGON_DANCE, MOVE_PETAL_DANCE, MOVE_CROSS_CHOP, MOVE_SPORE},
        .level = 13,
        .location = MAP_NUM(ROUTE3),
        .locationMap = MAPSEC_ROUTE_3
    },
    {
        .species = SPECIES_EEVEE,
        .moves = {MOVE_DRAGON_DANCE, MOVE_THIEF, MOVE_PSYCHIC, MOVE_ACID_ARMOR},
        .level = 17,
        .location = MAP_NUM(ROUTE8),
        .locationMap = MAPSEC_ROUTE_8
    },
    {
        .species = SPECIES_DUNSPARCE,
        .moves = {MOVE_SWORDS_DANCE, MOVE_FURY_CUTTER, MOVE_ROCK_SLIDE, MOVE_THRASH},
        .level = 18,
        .location = MAP_NUM(ROUTE7),
        .locationMap = MAPSEC_ROUTE_7
    },
    {
        .species = SPECIES_HOPPIP,
        .moves = {MOVE_DRAGON_DANCE, MOVE_FURY_CUTTER, MOVE_LEAF_BLADE, MOVE_FLY},
        .level = 5,
        .location = MAP_NUM(ROUTE1),
        .locationMap = MAPSEC_ROUTE_1
    },
    {
        .species = SPECIES_PARASECT,
        .moves = {MOVE_BELLY_DRUM, MOVE_FURY_CUTTER, MOVE_SPORE, MOVE_EXTREME_SPEED},
        .level = 50,
        .location = MAP_NUM(ROUTE18),
        .locationMap = MAPSEC_ROUTE_18
    },
};

void ClearOutbreakVars(void)
{
    VarSet(VAR_OUTBREAK_LEVEL, 0);
    VarSet(VAR_OUTBREAK_SPECIES, SPECIES_NONE);
    VarSet(VAR_OUTBREAK_MOVE_1, MOVE_NONE);
    VarSet(VAR_OUTBREAK_MOVE_2, MOVE_NONE);
    VarSet(VAR_OUTBREAK_MOVE_3, MOVE_NONE);
    VarSet(VAR_OUTBREAK_MOVE_4, MOVE_NONE);
    VarSet(VAR_OUTBREAK_LOCATION, 0);
    VarSet(VAR_OUTBREAK_LOCATION_MAP, 0);
}

void GenerateOutbreak(void)
{
    u16 outbreakIdx;
    u16 oldOutbreak = VarGet(VAR_YESTERDAYS_OUTBREAK);
    u16 oldOldOutbreak = VarGet(VAR_TWO_DAYS_AGO_OUTBREAK);
    u16 oldOldOldOutbreak = VarGet(VAR_THREE_DAYS_AGO_OUTBREAK);

    if (FlagGet(FLAG_DAILY_GENERATED_OUTBREAK)) // If flag is set, don't generate another outbreak. Flag gets cleared daily.
    {
        return;
    }

    do
    {
        outbreakIdx = Random() % ARRAY_COUNT(sPokeOutbreakSpeciesList);
    } while (sPokeOutbreakSpeciesList[outbreakIdx].species == oldOutbreak || 
             sPokeOutbreakSpeciesList[outbreakIdx].species == oldOldOutbreak || 
             sPokeOutbreakSpeciesList[outbreakIdx].species == oldOldOldOutbreak);

    VarSet(VAR_THREE_DAYS_AGO_OUTBREAK, oldOldOutbreak);
    VarSet(VAR_TWO_DAYS_AGO_OUTBREAK, oldOutbreak);
    VarSet(VAR_YESTERDAYS_OUTBREAK, sPokeOutbreakSpeciesList[outbreakIdx].species);
    VarSet(VAR_OUTBREAK_LEVEL, sPokeOutbreakSpeciesList[outbreakIdx].level);
    VarSet(VAR_OUTBREAK_SPECIES, sPokeOutbreakSpeciesList[outbreakIdx].species);
    VarSet(VAR_OUTBREAK_MOVE_1, sPokeOutbreakSpeciesList[outbreakIdx].moves[0]);
    VarSet(VAR_OUTBREAK_MOVE_2, sPokeOutbreakSpeciesList[outbreakIdx].moves[1]);
    VarSet(VAR_OUTBREAK_MOVE_3, sPokeOutbreakSpeciesList[outbreakIdx].moves[2]);
    VarSet(VAR_OUTBREAK_MOVE_4, sPokeOutbreakSpeciesList[outbreakIdx].moves[3]);
    VarSet(VAR_OUTBREAK_LOCATION, sPokeOutbreakSpeciesList[outbreakIdx].location);
    VarSet(VAR_OUTBREAK_LOCATION_MAP, sPokeOutbreakSpeciesList[outbreakIdx].locationMap);
    //show->massOutbreak.locationMapGroup = 3;
    //show->massOutbreak.probability = 50;
    FlagSet(FLAG_DAILY_GENERATED_OUTBREAK);
}
