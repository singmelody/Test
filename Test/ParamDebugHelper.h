#pragma once

struct sParamDebug_Avatar
{
	int32 nAvatarID;
};

typedef union ParamDebugUion
{
	char*	ParamBuffer;
	sParamDebug_Avatar*	pDebug_Avatar;
}ParamDebugUion;