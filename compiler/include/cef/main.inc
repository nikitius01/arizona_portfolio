#if defined _cef_main_inc
    #endinput
#endif
#define _cef_main_inc

#include cef/modals/main.inc
#include cef/svelte_apps/main.inc
#include cef/browser/main.inc
#include cef/ui_elements/main.inc

enum client_versions
{
    CLIENT_TYPE_VANILLA,
    CLIENT_TYPE_LAUNCHER,
    CLIENT_TYPE_MOBILE,
    CLIENT_TYPE_TRILOGY
};
new client_versions:g_player_client_version[MAX_PLAYERS];

stock IsLauncherConnected(playerid)
	return g_player_client_version[playerid] == CLIENT_TYPE_LAUNCHER;

stock IsMobileConnected(playerid)
	return g_player_client_version[playerid] == CLIENT_TYPE_MOBILE;

stock IsTrilogyConnected(playerid)
	return g_player_client_version[playerid] == CLIENT_TYPE_TRILOGY;

//
enum e_front_messages
{
	e_front_message_event[32],
	e_front_message_elementid,
	e_front_message_subtype,
	bool:e_front_message_encoded
};
new g_front_messages[][e_front_messages] =
{
	{"event.donateShop.updateProduct", UI_DONATE_SHOP, 1, true}
};

const sizeofs_front_messages = sizeof(g_front_messages);

new g_front_message_str[1024];

stock SendMessageToFront(playerid, const front_event[], const message[])
{
	new index = -1;

	for(new i; i < sizeofs_front_messages; i ++)
	{
		if(!GetString(g_front_messages[i][e_front_message_event], front_event))
			continue;

		index = i;

		break;
	}

	if(index != -1)
	{
		if(IsMobileConnected(playerid))
		{
			UIElement:ExecuteEvent
			(
				playerid,
				g_front_messages[index][e_front_message_elementid],
				g_front_messages[index][e_front_message_subtype],
				_:g_front_messages[index][e_front_message_encoded],
				message
			);
		}
		else
		{
			format(g_front_message_str, 1024, "window.executeEvent('%s', `[%s]`);", front_event, message);
			Svelte:ExecuteEvent(playerid, _:g_front_messages[index][e_front_message_encoded], g_front_message_str);
		}

		printf("SendMessageToFront(playerid, front_event: [%d] %s (elementid: %d));", g_front_messages[index][e_front_message_subtype], front_event, g_front_messages[index][e_front_message_elementid]);
		printf("message: %s", message);
	}
	else
		printf("(SendMessageToFront): не удалось определить index");

	return true;
}

//
enum PR_JoinData
{
	PR_iVersion,
	PR_byteMod,
	PR_byteNicknameLen,
	PR_NickName[24],
	PR_uiClientChallengeResponse,
	PR_byteAuthKeyLen,
	PR_auth_key[50],
	PR_iClientVerLen,
	PR_ClientVersion[30]
};

const PLAYER_SERVER_JOIN = 25;

IRPC:PLAYER_SERVER_JOIN(playerid, BitStream:bs)
{
	new JoinData[PR_JoinData];
	BS_ReadJoinServer(bs, JoinData);

	strcopy(g_player_name[playerid], JoinData[PR_NickName]);

    if(GetString(JoinData[PR_ClientVersion], "Arizona PC")){

		g_player_client_version[playerid] = CLIENT_TYPE_LAUNCHER;

	}
    else if(GetString(JoinData[PR_auth_key], "39FB2DEEDB49ACFB8D4EECE6953D2507988CCCF4410")){

		g_player_client_version[playerid] = CLIENT_TYPE_MOBILE;
		
	}
    else if(JoinData[PR_iClientVerLen] >= 20 && GetString(JoinData[PR_auth_key], "15121F6F18550C00AC4B4F8A167D0379BB0ACA99043")) {

		g_player_client_version[playerid] = CLIENT_TYPE_TRILOGY;

		strcopy(JoinData[PR_ClientVersion], "Arizona Trilogy");

		JoinData[PR_iClientVerLen] = strlen(JoinData[PR_ClientVersion]);
	}
    else {

        g_player_client_version[playerid] = CLIENT_TYPE_VANILLA;

	}

	printf("Player %s joined, join_data ->\n\
		PR_iVersion = %d\n\
		PR_byteMod = %d\n\
		PR_NickName = %s (len: %d)\n\
		PR_uiClientChallengeResponse = %d\n\
		PR_auth_key = %s (len: %d)\n\
		PR_ClientVersion = %s (len: %d)", 
		Name(playerid),
		JoinData[PR_iVersion],
		JoinData[PR_byteMod],
		JoinData[PR_NickName], JoinData[PR_byteNicknameLen],
		JoinData[PR_uiClientChallengeResponse],
		JoinData[PR_auth_key], JoinData[PR_byteAuthKeyLen],
		JoinData[PR_ClientVersion], JoinData[PR_iClientVerLen]
	);

	BS_Reset(bs);

	BS_WriteJoinServer(bs, JoinData);

	return true;
}

stock BS_ReadJoinServer(BitStream:bs, data[PR_JoinData])
{
    BS_ReadValue(bs,
        PR_INT32, data[PR_iVersion],
		PR_UINT8, data[PR_byteMod],
		PR_UINT8, data[PR_byteNicknameLen],
		PR_STRING, data[PR_NickName], data[PR_byteNicknameLen],
		PR_UINT32, data[PR_uiClientChallengeResponse],
		PR_UINT8, data[PR_byteAuthKeyLen],
		PR_STRING, data[PR_auth_key], data[PR_byteAuthKeyLen],
		PR_UINT8, data[PR_iClientVerLen]
	);

	BS_ReadValue(bs,
		PR_STRING, data[PR_ClientVersion], (data[PR_iClientVerLen] >= 30 ? 30:data[PR_iClientVerLen]) 
	);
}

stock BS_WriteJoinServer(BitStream:bs, data[PR_JoinData])
{
    BS_WriteValue(bs,
        PR_UINT32, data[PR_iVersion],
		PR_UINT8, data[PR_byteMod],
		PR_UINT8, data[PR_byteNicknameLen],
		PR_STRING, data[PR_NickName],
		PR_UINT32, data[PR_uiClientChallengeResponse],
		PR_UINT8, data[PR_byteAuthKeyLen],
		PR_STRING, data[PR_auth_key],
		PR_UINT8, data[PR_iClientVerLen],
		PR_STRING, data[PR_ClientVersion]
	);
}

public OnPlayerDisconnect(playerid, reason)
{
	// main
    g_player_client_version[playerid] = CLIENT_TYPE_VANILLA;
    
    // modals
	for(new i; i < MAX_MODAL_TYPES; i ++)
		g_player_modal_type_opened[playerid][i] = false;
    
    // svelte_apps
    g_player_svelte_app[playerid] = SVELTE_APP_NONE;
    g_player_svelte_app_browser[playerid] = -1;
    g_player_svelte_allowed_esc[playerid] = false;
    
    // browser
    for(new i; i < MAX_ACTIVE_BROWSERS; i++)
        g_player_browser_cursor[playerid][i] = false;

    Iter_Clear(g_player_active_browsers[playerid]);

    g_player_screen_resolution[playerid][0] = 0;
    g_player_screen_resolution[playerid][1] = 0;
    
    // ui_elements
    g_player_ui_element[playerid] = UI_ELEMENT_NONE;
    g_player_ui_hide_allowed[playerid] = false;

    #if defined cef_OnPlayerDisconnect
        return cef_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect cef_OnPlayerDisconnect
#if defined cef_OnPlayerDisconnect
    forward cef_OnPlayerDisconnect(playerid, reason);
#endif