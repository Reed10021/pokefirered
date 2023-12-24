#include "global.h"
#include "lottery_corner.h"
#include "event_data.h"
#include "pokemon.h"
#include "constants/items.h"
#include "random.h"
#include "constants/species.h"
#include "string_util.h"
#include "text.h"
#include "pokemon_storage_system.h"

static EWRAM_DATA u16 sWinNumberDigit = 0;
static EWRAM_DATA u16 sOtIdDigit = 0;

static const u16 sLotteryPrizes[] =
{
    ITEM_PP_UP,
    ITEM_EXP_SHARE,
    ITEM_MAX_REVIVE,
    ITEM_MASTER_BALL,
};

static const u16 sLotteryPrizes1Digit[] =
{
    ITEM_ORAN_BERRY,
    ITEM_SITRUS_BERRY,
    ITEM_LEPPA_BERRY,
    ITEM_LUM_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_GANLON_BERRY,
    ITEM_SALAC_BERRY,
    ITEM_PETAYA_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_LANSAT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_ABILITY_CAPSULE,
    ITEM_RARE_CANDY,
    ITEM_PP_MAX,
};

#define LOTTERY_ONE_DIGIT_MAX_SIZE 14


static u8 GetMatchingDigits(u16, u16);

void ResetLotteryCorner(void)
{
    u16 rand = Random();

    SetLotteryNumber((Random() << 16) | rand);
    VarSet(VAR_POKELOT_PRIZE_ITEM, 0);
}

void SetRandomLotteryNumber(u16 i)
{
    u32 var = Random();

    while (--i != 0xFFFF)
        var = ISO_RANDOMIZE2(var);

    SetLotteryNumber(var);
}

void RetrieveLotteryNumber(void)
{
    u16 lottoNumber = GetLotteryNumber();
    gSpecialVar_Result = lottoNumber;
}

void PickLotteryCornerTicket(void)
{
    u16 i;
    u16 j;
    u32 box;
    u32 slot;
    u32 slotForRandom;

    gSpecialVar_0x8004 = 0;
    slot = 0;
    box = 0;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE)
        {
            // do not calculate ticket values for eggs.
            if (!GetMonData(mon, MON_DATA_IS_EGG))
            {
                u32 otId = GetMonData(mon, MON_DATA_OT_ID);
                u8 numMatchingDigits = GetMatchingDigits(gSpecialVar_Result, otId);
                slotForRandom = i;

                if (numMatchingDigits > gSpecialVar_0x8004 && numMatchingDigits > 0)
                {
                    gSpecialVar_0x8004 = numMatchingDigits;
                    box = TOTAL_BOXES_COUNT;
                    slot = i;
                }
            }
        }
        else // pokemon are always arranged from populated spots first to unpopulated, so the moment a NONE species is found, that's the end of the list.
            break;
    }

    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (GetBoxMonData(&gPokemonStoragePtr->boxes[i][j], MON_DATA_SPECIES) != SPECIES_NONE &&
            !GetBoxMonData(&gPokemonStoragePtr->boxes[i][j], MON_DATA_IS_EGG))
            {
                u32 otId = GetBoxMonData(&gPokemonStoragePtr->boxes[i][j], MON_DATA_OT_ID);
                u8 numMatchingDigits = GetMatchingDigits(gSpecialVar_Result, otId);

                if (numMatchingDigits > gSpecialVar_0x8004 && numMatchingDigits > 0)
                {
                    gSpecialVar_0x8004 = numMatchingDigits;
                    box = i;
                    slot = j;
                }
            }
        }
    }

    // 66% of the time, change a bad no win result into a winning result
    if (gSpecialVar_0x8004 == 0)
    {
        if (Random() % 3 > 0) {
            if (Random() % 2 == 0) {
                if (Random() % 2 == 0) {
                    if (Random() % 2 == 0) {
                        if (Random() % 4 == 0) {
                            gSpecialVar_0x8004 = 5;
                        } else
                            gSpecialVar_0x8004 = 4;
                    } else
                        gSpecialVar_0x8004 = 3;
                } else
                    gSpecialVar_0x8004 = 2;
            } else
                gSpecialVar_0x8004 = 1;
            box = TOTAL_BOXES_COUNT;
            slot = slotForRandom;
        }
        // Else don't change result
    }

    if (gSpecialVar_0x8004 != 0)
    {
        if (gSpecialVar_0x8004 == 1) {
            gSpecialVar_0x8005 = sLotteryPrizes1Digit[Random() % LOTTERY_ONE_DIGIT_MAX_SIZE];
            gSpecialVar_0x800A = 3; // Count: 3
        }
        else {
            gSpecialVar_0x8005 = sLotteryPrizes[gSpecialVar_0x8004 - 2]; // We don't use 0, 1 is handled above, sLotteryPrizes starts at element 0 so subtract 2
            if (gSpecialVar_0x8004 == 2)
                gSpecialVar_0x800A = 5; // Count: 5
            else if (gSpecialVar_0x8004 == 3)
                gSpecialVar_0x800A = 1; // Count: 1
            else if (gSpecialVar_0x8004 == 4)
                gSpecialVar_0x800A = 10; // Count: 10
            else if (gSpecialVar_0x8004 == 5)
                gSpecialVar_0x800A = 2; // Count: 2
        }

        if (box == TOTAL_BOXES_COUNT)
        {
            gSpecialVar_0x8006 = 0;
            GetMonData(&gPlayerParty[slot], MON_DATA_NICKNAME, gStringVar1);
        }
        else
        {
            gSpecialVar_0x8006 = 1;
            GetBoxMonData(&gPokemonStoragePtr->boxes[box][slot], MON_DATA_NICKNAME, gStringVar1);
        }
        StringGet_Nickname(gStringVar1);
    }
}

static u8 GetMatchingDigits(u16 winNumber, u16 otId)
{
    u8 i;
    u8 matchingDigits = 0;

    for (i = 0; i < 5; i++)
    {
        sWinNumberDigit = winNumber % 10;
        sOtIdDigit = otId % 10;

        if (sWinNumberDigit == sOtIdDigit)
        {
            winNumber = winNumber / 10;
            otId = otId / 10;
            matchingDigits++;
        }
        else
            break;
    }
    return matchingDigits;
}

// lottery numbers go from 0 to 99999, not 65535 (0xFFFF). interestingly enough, the function that calls GetLotteryNumber shifts to u16, so it cant be anything above 65535 anyway.
void SetLotteryNumber(u32 lotteryNum)
{
    u16 lowNum = lotteryNum >> 16;
    u16 highNum = lotteryNum;

    VarSet(VAR_POKELOT_RND1, highNum);
    VarSet(VAR_POKELOT_RND2, lowNum);
}

u32 GetLotteryNumber(void)
{
    u16 highNum = VarGet(VAR_POKELOT_RND1);
    u16 lowNum = VarGet(VAR_POKELOT_RND2);

    return (lowNum << 16) | highNum;
}

// interestingly, this may have been the original lottery number set function, but GF tried to change it to 32-bit later but didnt finish changing all calls as one GetLotteryNumber still shifts to u16.
void SetLotteryNumber16_Unused(u16 lotteryNum)
{
    SetLotteryNumber(lotteryNum);
}
