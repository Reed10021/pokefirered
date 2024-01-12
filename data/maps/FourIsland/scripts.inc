.equ LOCALID_DAYCARE_MAN, 1
.equ LOCALID_RIVAL, 10

FourIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FourIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FourIsland_OnFrame
	.byte 0

FourIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FOUR_ISLAND
	call_if_eq VAR_MAP_SCENE_FOUR_ISLAND, 0, FourIsland_EventScript_ShowRival
	call FourIsland_EventScript_TrySetDayCareManPos
	end

FourIsland_EventScript_ShowRival::
	clearflag FLAG_HIDE_FOUR_ISLAND_RIVAL
	return

FourIsland_EventScript_TrySetDayCareManPos::
	goto_if_unset FLAG_PENDING_DAYCARE_EGG, FourIsland_EventScript_EndSetDayCareManPos
	setobjectxyperm LOCALID_DAYCARE_MAN, 16, 14
FourIsland_EventScript_EndSetDayCareManPos::
	return

FourIsland_OnFrame::
	map_script_2 VAR_MAP_SCENE_FOUR_ISLAND, 0, FourIsland_EventScript_RivalScene
	.2byte 0

FourIsland_EventScript_RivalScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_RIVAL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playbgm MUS_ENCOUNTER_RIVAL, 0
	applymovement LOCALID_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVAL, Common_Movement_Delay48
	waitmovement 0
	delay 25
	applymovement LOCALID_RIVAL, FourIsland_Movement_RivalApproach
	waitmovement 0
	msgbox FourIsland_Text_RivalAlreadyGotEggBeSmellingYa
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, FourIsland_Movement_PlayerWatchRivalExit
	applymovement LOCALID_RIVAL, FourIsland_Movement_RivalExit
	waitmovement 0
	fadedefaultbgm
	playse SE_EXIT
	delay 35
	removeobject LOCALID_RIVAL
	setvar VAR_MAP_SCENE_FOUR_ISLAND, 1
	releaseall
	end

FourIsland_Movement_RivalApproach::
	walk_right
	walk_right
	walk_down
	walk_down
	step_end

FourIsland_Movement_RivalExit::
	delay_16
	delay_4
	walk_down
	step_end

FourIsland_Movement_PlayerWatchRivalExit::
	walk_left
	walk_in_place_faster_right
	step_end

FourIsland_EventScript_DaycareMan_Movement::
	goto_if_unset FLAG_PENDING_DAYCARE_EGG, FourIsland_EventScript_DaycareMan_Movement_MoveBack
	getobjectxy LOCALID_DAYCARE_MAN, 0
	goto_if_ge VAR_0x8008, 14, FourIsland_EventScript_DaycareMan_Movement_End
	goto_if_eq VAR_RESULT, FALSE, FourIsland_EventScript_DaycareMan_Movement_OffMap

	@ If Y is already >=14, dont move down (south).
	checkobjectat 16, 14
	goto_if_eq VAR_RESULT, TRUE, FourIsland_EventScript_DaycareMan_Movement_End
	@ If an object is already at X = 16 Y = 14, dont move.
	applymovement LOCALID_DAYCARE_MAN, FourIsland_Movement_DaycareMan_WalkDown
	waitmovement LOCALID_DAYCARE_MAN
	setobjectxyperm LOCALID_DAYCARE_MAN, 16, 14
	goto FourIsland_EventScript_DaycareMan_Movement_End

FourIsland_EventScript_DaycareMan_Movement_OffMap:
	setobjectxyperm LOCALID_DAYCARE_MAN, 16, 14
	goto FourIsland_EventScript_DaycareMan_Movement_End

FourIsland_EventScript_DaycareMan_Movement_MoveBack:
	getobjectxy LOCALID_DAYCARE_MAN, 0
	goto_if_le VAR_0x8008, 13, FourIsland_EventScript_DaycareMan_Movement_End
	goto_if_eq VAR_RESULT, FALSE, Route117_EventScript_DaycareMan_Movement_OffMap_MoveBack
	@ If Y is already <=13, dont move up (north).
	checkobjectat 16, 13
	goto_if_eq VAR_RESULT, TRUE, FourIsland_EventScript_DaycareMan_Movement_End
	@ If an object is already at X = 16 Y = 13, dont move.
	applymovement LOCALID_DAYCARE_MAN, FourIsland_Movement_DaycareMan_WalkUp
	waitmovement LOCALID_DAYCARE_MAN
	setobjectxyperm LOCALID_DAYCARE_MAN, 16, 13
	goto FourIsland_EventScript_DaycareMan_Movement_End

Route117_EventScript_DaycareMan_Movement_OffMap_MoveBack:
	setobjectxyperm LOCALID_DAYCARE_MAN, 16, 13

FourIsland_EventScript_DaycareMan_Movement_End:
	end

FourIsland_Movement_DaycareMan_WalkDown::
	walk_down
	step_end

FourIsland_Movement_DaycareMan_WalkUp::
	walk_up
	walk_in_place_faster_down
	step_end

FourIsland_EventScript_DaycareMan::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	special GetDaycareMonNicknames
	specialvar VAR_RESULT, GetDaycareState
	goto_if_eq VAR_RESULT, DAYCARE_EGG_WAITING, FourIsland_EventScript_DaycareEggWaiting
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, FourIsland_EventScript_CheckOnOneMon
	goto_if_eq VAR_RESULT, DAYCARE_TWO_MONS, FourIsland_EventScript_CheckOnTwoMons
	msgbox DayCare_Text_ImDaycareManSpeakToMyWife
	compare VAR_EGG_CHAIN, 0
	goto_if_eq NoEggChain
	buffernumberstring 0, VAR_EGG_CHAIN
	msgbox DayCare_Text_EggChainInfo, MSGBOX_DEFAULT
NoEggChain:
	compare VAR_CHAIN, 3
	goto_if_lt NoChain
	bufferspeciesname 0, VAR_SPECIESCHAINED
	buffernumberstring 1, VAR_CHAIN
	msgbox DayCare_Text_NormalChainInfo, MSGBOX_DEFAULT
NoChain:
	release
	end

FourIsland_EventScript_DaycareEggWaiting::
	msgbox DayCare_Text_DoYouWantEgg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FourIsland_EventScript_DaycareAcceptEgg
	msgbox DayCare_Text_IWillKeepDoYouWantIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FourIsland_EventScript_DaycareAcceptEgg
	compare VAR_0x8009, 5
	goto_if_ne NotShiny @ If equal, then prompt about special egg
	msgbox DayCare_Text_EggIsSpecialAreYouSure, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq FourIsland_EventScript_DaycareAcceptEgg
NotShiny:
	msgbox DayCare_Text_IllKeepIt
	clearflag FLAG_PENDING_DAYCARE_EGG
	special RejectEggFromDayCare
	buffernumberstring 0, VAR_EGG_CHAIN
	msgbox DayCare_Text_EggChainInfo, MSGBOX_DEFAULT
	compare VAR_CHAIN, 3
	goto_if_lt NotChaining1
	bufferspeciesname 0, VAR_SPECIESCHAINED
	buffernumberstring 1, VAR_CHAIN
	msgbox DayCare_Text_NormalChainInfo, MSGBOX_DEFAULT
NotChaining1:
	closemessage
	call FourIsland_EventScript_DaycareMan_Movement
	release
	end

FourIsland_EventScript_DaycareAcceptEgg::
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	goto_if_ne VAR_RESULT, PARTY_SIZE, FourIsland_EventScript_DaycareReceivedEgg
	msgbox DayCare_Text_YouHaveNoRoomForIt
	release
	end

FourIsland_EventScript_DaycareReceivedEgg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message DayCare_Text_ReceivedEgg
	call EventScript_RestorePrevTextColor
	playfanfare MUS_LEVEL_UP
	waitfanfare
	waitbuttonpress
	msgbox DayCare_Text_TakeGoodCareOfIt
	special GiveEggFromDaycare
	clearflag FLAG_PENDING_DAYCARE_EGG
	buffernumberstring 0, VAR_EGG_CHAIN
	msgbox DayCare_Text_EggChainInfo, MSGBOX_DEFAULT
	compare VAR_CHAIN, 3
	goto_if_lt NoChaining
	bufferspeciesname 0, VAR_SPECIESCHAINED
	buffernumberstring 1, VAR_CHAIN
	msgbox DayCare_Text_NormalChainInfo, MSGBOX_DEFAULT
NoChaining:
	closemessage
	call FourIsland_EventScript_DaycareMan_Movement
	release
	end

FourIsland_EventScript_CheckOnOneMon::
	special GetDaycareMonNicknames
	msgbox DayCare_Text_YourMonIsDoingFine
	compare VAR_EGG_CHAIN, 0
	goto_if_eq NoEggChaining
	buffernumberstring 0, VAR_EGG_CHAIN
	msgbox DayCare_Text_EggChainInfo, MSGBOX_DEFAULT
NoEggChaining:
	compare VAR_CHAIN, 3
	goto_if_lt SkipChaining
	bufferspeciesname 0, VAR_SPECIESCHAINED
	buffernumberstring 1, VAR_CHAIN
	msgbox DayCare_Text_NormalChainInfo, MSGBOX_DEFAULT
SkipChaining:
	release
	end

FourIsland_EventScript_CheckOnTwoMons::
	special GetDaycareMonNicknames
	msgbox DayCare_Text_YourMonsAreDoingFine
	special SetDaycareCompatibilityString
	special ShowFieldMessageStringVar4
	waitmessage
	waitbuttonpress
	compare VAR_EGG_CHAIN, 0
	goto_if_eq NotEggChaining
	buffernumberstring 0, VAR_EGG_CHAIN
	msgbox DayCare_Text_EggChainInfo, MSGBOX_DEFAULT
NotEggChaining:
	compare VAR_CHAIN, 3
	goto_if_lt NotChaining
	bufferspeciesname 0, VAR_SPECIESCHAINED
	buffernumberstring 1, VAR_CHAIN
	msgbox DayCare_Text_NormalChainInfo, MSGBOX_DEFAULT
NotChaining:
	release
	end

FourIsland_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, FourIsland_EventScript_OldWomanLoreleiLeft
	msgbox FourIsland_Text_LoreleiHasReturned
	release
	end

FourIsland_EventScript_OldWomanLoreleiLeft::
	famechecker FAMECHECKER_LORELEI, 5
	msgbox FourIsland_Text_LoreleiMetLaprasAsChild
	release
	end

FourIsland_EventScript_LittleGirl::
	lock
	faceplayer
	famechecker FAMECHECKER_LORELEI, 4
	msgbox FourIsland_Text_LoreleiHasLotsOfStuffedDolls
	release
	end

FourIsland_EventScript_FatMan::
	msgbox FourIsland_Text_MadeSlushieUsingIcefallCaveIce, MSGBOX_NPC
	end

FourIsland_EventScript_IslandSign::
	msgbox FourIsland_Text_IslandSign, MSGBOX_SIGN
	end

FourIsland_EventScript_LoreleisHouseSign::
	msgbox FourIsland_Text_LoreleisHouse, MSGBOX_SIGN
	end
