PalletTown_PlayersHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_PlayersHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PalletTown_PlayersHouse_2F_OnWarp
	.byte 0

PalletTown_PlayersHouse_2F_OnTransition::
	call_if_eq VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_EventScript_SetRespawn
	call_if_unset FLAG_SET_WALL_CLOCK, PalletTown_PlayersHouse_2F_EventScript_SetTempVar
	end

PalletTown_PlayersHouse_2F_EventScript_SetRespawn::
	setrespawn SPAWN_PALLET_TOWN
	return

PalletTown_PlayersHouse_2F_EventScript_SetTempVar::
	setvar VAR_TEMP_1, 5
	return

PalletTown_PlayersHouse_2F_OnWarp::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_FirstWarpIn
	.2byte 0

PalletTown_PlayersHouse_2F_FirstWarpIn::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 1
	end

PalletTown_PlayersHouse_2F_EventScript_NES::
	msgbox PalletTown_PlayersHouse_2F_Text_PlayedWithNES, MSGBOX_SIGN
	end

PalletTown_PlayersHouse_2F_EventScript_Sign::
	msgbox PalletTown_PlayersHouse_2F_Text_PressLRForHelp, MSGBOX_SIGN
	end

PalletTown_PlayersHouse_2F_EventScript_SetClock::
	goto_if_set FLAG_SET_WALL_CLOCK, PalletTown_PlayersHouse_2F_EventScript_SetClock_End
	lockall
	checkplayergender
	compare VAR_RESULT, MALE
	goto_if_eq PalletTown_PlayersHouse_2F_EventScript_SetClock_Male
	setvar VAR_0x8004, FEMALE
	goto PalletTown_PlayersHouse_2F_EventScript_SetClock_Resume
PalletTown_PlayersHouse_2F_EventScript_SetClock_Male:
	setvar VAR_0x8004, MALE
PalletTown_PlayersHouse_2F_EventScript_SetClock_Resume:
	msgbox PalletTown_PlayersHouse_2F_Text_ClockIsStopped, MSGBOX_DEFAULT
	call PalletTown_PlayersHouse_2F_EventScript_SetWallClock
	delay 30
	setflag FLAG_SET_WALL_CLOCK
	setvar VAR_TEMP_1, 0
	releaseall
PalletTown_PlayersHouse_2F_EventScript_SetClock_End:
	end

PalletTown_PlayersHouse_2F_EventScript_SetWallClock::
	fadescreen FADE_TO_BLACK
	special StartWallClock
	waitstate
	return

PalletTown_PlayersHouse_2F_EventScript_PC::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	setvar VAR_0x8004, HELPCONTEXT_BEDROOM_PC
	special Script_SetHelpContext
	setvar VAR_0x8004, 1
	special AnimatePcTurnOn
	playse SE_PC_ON
	msgbox Text_PlayerBootedUpPC
	special BedroomPC
	waitstate
	special SetHelpContextForMap
	releaseall
	end

EventScript_PalletTown_PlayersHouse_2F_ShutDownPC::
	setvar VAR_0x8004, 1
	playse SE_PC_OFF
	special AnimatePcTurnOff
	special SetHelpContextForMap
	releaseall
	end

PalletTown_PlayersHouse_2F_Text_ClockIsStopped:
	.string "Whoops, the clock is stopped.\p"
	.string "Better set it and start it!$"
