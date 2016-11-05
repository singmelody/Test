#pragma once

#define AVATAR_STATE_START	0

enum{
	eGAS_Unkown = -1,

	eGAS_IDLE	= AVATAR_STATE_START,	
	eGAS_MOVE,
	eGAS_SPRINT,
	eGAS_ROLL,
	eGAS_JUMP,

	eGAS_State_Size				= 37,

	eGAS_Base_NewConnection,
	eGAS_Base_CltCreateAvatar,
	eGAS_Base_Dizzy,
	eGAS_Base_Avatar_Frenzy,

	eGAS_COUNT,
};