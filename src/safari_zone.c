#include "global.h"
#include "battle.h"
#include "event_scripts.h"
#include "overworld.h"
#include "pokemon.h"
#include "script.h"
#include "string_util.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "field_specials.h"

extern const u8 ChainNumber[];
extern const u8 DeleteChain[];

EWRAM_DATA u8 gNumSafariBalls = 0;
EWRAM_DATA u16 gSafariZoneStepCounter = 0;

bool32 GetSafariZoneFlag(void)
{
    return FlagGet(FLAG_SYS_SAFARI_MODE);
}

void SetSafariZoneFlag(void)
{
    FlagSet(FLAG_SYS_SAFARI_MODE);
}

void ResetSafariZoneFlag(void)
{
    FlagClear(FLAG_SYS_SAFARI_MODE);
}

void EnterSafariMode(void)
{
    IncrementGameStat(GAME_STAT_ENTERED_SAFARI_ZONE);
    SetSafariZoneFlag();
    gNumSafariBalls = 30;
    gSafariZoneStepCounter = 900;
}

void ExitSafariMode(void)
{
    ResetSafariZoneFlag();
    gNumSafariBalls = 0;
    gSafariZoneStepCounter = 0;
}

bool8 SafariZoneTakeStep(void)
{
    if (GetSafariZoneFlag() == FALSE)
        return FALSE;
    gSafariZoneStepCounter--;
    if (gSafariZoneStepCounter == 0)
    {
        ScriptContext_SetupScript(SafariZone_EventScript_TimesUp);
        return TRUE;
    }
    return FALSE;
}

void SafariZoneRetirePrompt(void)
{
    ScriptContext_SetupScript(SafariZone_EventScript_RetirePrompt);
}

void CB2_EndSafariBattle(void)
{
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES);
    u16 chainCount = VarGet(VAR_CHAIN);

    // If in the safari zone, be a little more generous with chaining.
    // We can chain with a successful catch or a 'mon fleeing.
    // We do not reset the chain on running out of balls, but do on the trainer running away
    if (gBattleOutcome == B_OUTCOME_CAUGHT || gBattleOutcome == B_OUTCOME_MON_FLED)
    {
        // if we have a species, the species wasn't correct, and the chain is not zero, yeet.
        if (species != VarGet(VAR_SPECIESCHAINED) && chainCount != 0)
        {
            // If the chain was 3, show textbox showing you messed up.
            if (chainCount >= 3)
            {
                u8 numDigits = CountDigits(chainCount);
                ConvertIntToDecimalStringN(gStringVar1, chainCount, STR_CONV_MODE_LEFT_ALIGN, numDigits);
                GetSpeciesName(gStringVar2, VarGet(VAR_SPECIESCHAINED));
                ScriptContext_SetupScript(DeleteChain);
                // Cleanup
                VarSet(VAR_CHAIN, 0);
                VarSet(VAR_SPECIESCHAINED, 0);
            }
            else // if the chain wasn't +3, then act like we've started chaining this new species and are incrementing the counter.
            {
                VarSet(VAR_SPECIESCHAINED, species);
                VarSet(VAR_CHAIN, 1);
            }
        }
        else
        {
            // if no chain, start chaining
            if (VarGet(VAR_SPECIESCHAINED) == 0)
                VarSet(VAR_SPECIESCHAINED, species);
            // if chain, increment chain and maybe show text
            if (species == VarGet(VAR_SPECIESCHAINED))
            {
                // If we caught a chained pokemon, increase the chain by 6 (5 here + the normal 1).
                // The ChainNumber script contains a check for 0xFFFF, so if we increase by 5 here it won't increase again.
                if (gBattleOutcome == B_OUTCOME_CAUGHT && chainCount >= 3 && chainCount <= 0xFFFA)
                    VarSet(VAR_CHAIN, chainCount + 5);

                GetSpeciesName(gStringVar2, species);
                ScriptContext_SetupScript(ChainNumber);
            }
        }
    }
    else if (gBattleOutcome == B_OUTCOME_RAN)
    {
        // If we had a chain and the species was correct but we ran from it.
        if (chainCount != 0 && species == VarGet(VAR_SPECIESCHAINED))
        {
            if (chainCount >= 3)
            {
                u8 numDigits = CountDigits(chainCount);
                ConvertIntToDecimalStringN(gStringVar1, chainCount, STR_CONV_MODE_LEFT_ALIGN, numDigits);
                GetSpeciesName(gStringVar2, VarGet(VAR_SPECIESCHAINED));
                ScriptContext_SetupScript(DeleteChain);
            }
            VarSet(VAR_CHAIN, 0);
            VarSet(VAR_SPECIESCHAINED, 0);
        }
    }

    if (gNumSafariBalls != 0)
    {
        SetMainCallback2(CB2_ReturnToField);
    }
    else if (gBattleOutcome == B_OUTCOME_NO_SAFARI_BALLS)
    {
        RunScriptImmediately(SafariZone_EventScript_OutOfBallsMidBattle);
        WarpIntoMap();
        gFieldCallback = FieldCB_SafariZoneRanOutOfBalls;
        SetMainCallback2(CB2_LoadMap);
    }
    else if (gBattleOutcome == B_OUTCOME_CAUGHT)
    {
        ScriptContext_SetupScript(SafariZone_EventScript_OutOfBalls);
        ScriptContext_Stop();
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
    }
}
