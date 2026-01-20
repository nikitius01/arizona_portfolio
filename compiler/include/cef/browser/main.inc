#if defined _cef_browser_inc
    #endinput
#endif
#define _cef_browser_inc

#define MAX_ACTIVE_BROWSERS (10)

new bool:g_player_browser_cursor[MAX_PLAYERS][MAX_ACTIVE_BROWSERS];

new IteratorArray:g_player_active_browsers[MAX_PLAYERS]<MAX_ACTIVE_BROWSERS>;
new g_player_screen_resolution[MAX_PLAYERS][2];

stock SetCursorBrowser(playerid, index, bool:enable)
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return false;

    else if(!(0 <= index < MAX_ACTIVE_BROWSERS))
        return false;

    SendClientMessage(playerid, -1, "SetCursorBrowser(playerid = %d, index = %d, enable = %b)", playerid, index, enable);

    g_player_browser_cursor[playerid][index] = enable;

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 220,
		PR_UINT8, 25,
		PR_UINT32, index,
		PR_BOOL, enable
    );

	return PR_SendPacket(bs, playerid), BS_Delete(bs);
}

stock CreatePlayerBrowser
(
    playerid, screen_1 = 0, screen_2 = 0,

    const url[], const svelteKey[],

    create_type = 10, player_object_id = -1, Float:radius = 0.0,

    encode_url = 0, encode_key = 0
) {
    new browserid = Iter_Free(g_player_active_browsers[playerid]);

    if(browserid != ITER_NONE && Iter_Add(g_player_active_browsers[playerid], browserid))
    {
        printf("CreatePlayerBrowser(playerid = %d, url = %s, key = %s)", playerid, url, svelteKey);

        new BitStream:bs = BS_New();

        BS_WriteValue(bs,
            PR_UINT8, 220,
            PR_UINT8, create_type
        );

        if(create_type != 14)
        {
            if(screen_1 < 1) screen_1 = g_player_screen_resolution[playerid][0];
            if(screen_2 < 1) screen_2 = g_player_screen_resolution[playerid][1];

            BS_WriteValue(bs,
                PR_UINT32, screen_1,
                PR_UINT32, screen_2,
                PR_UINT32, 0,
                PR_UINT32, 0,
                PR_UINT16, strlen(url) + encode_url,
                PR_UINT8, encode_url,
                encode_url ? PR_CSTRING:PR_STRING, url,
                PR_UINT16, strlen(svelteKey) + encode_key,
                PR_UINT8, encode_key,
                encode_key ? PR_CSTRING:PR_STRING, svelteKey
            );

            if(create_type == 12)
            {
                BS_WriteValue(bs,
                    PR_UINT16, player_object_id,
                    PR_UINT16, GetPlayerObjectModel(playerid, player_object_id),
                    PR_FLOAT, radius
                );
            }
        }
        BS_WriteValue(bs, PR_UINT32, browserid);

        PR_SendPacket(bs, playerid), BS_Delete(bs);
    }

    return browserid;
}

public OnIncomingPacket(playerid, packetid, BitStream:bs) {
 
    if (!(0 <= playerid < MAX_PLAYERS)) {
 
        return 1;
    }

    if(packetid == 220)
    {
        BS_IgnoreBits(bs, 8);
 
        new g_action_id = 0;
 
        BS_ReadValue(bs, PR_UINT8, g_action_id);

        if(g_action_id == 20)
        {
			BS_ReadValue(bs,
				PR_UINT32, g_player_screen_resolution[playerid][0],
				PR_UINT32, g_player_screen_resolution[playerid][1]
            );

            printf("resolution = [%d, %d]", g_player_screen_resolution[playerid][0], g_player_screen_resolution[playerid][1]);

			new svelte = CreatePlayerBrowser(playerid, g_player_screen_resolution[playerid][0], g_player_screen_resolution[playerid][1], "file:///frontend/svelte_js/index.html", "or7lq8Q8heur");

            if(svelte != ITER_NONE)
                SetPlayerSvelteAppBrowserId(playerid, svelte);
            else
                printf("[%s(%d)] Не удалось создать svelte браузер игроку %d", __file, __line, playerid);
        }
        else if(g_action_id == 18)
        {
            if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
                return false;

            new svelte_message[256], length;

			BS_ReadValue(bs, PR_UINT16, length);

            if(length <= 0 || length >= sizeof(svelte_message))
            {
                printf("[SvelteMessage] Ошибка при чтении пакета (length < 0 || length > %d)", sizeof(svelte_message));

                return false;
            }
            else
                BS_ReadValue(bs, PR_STRING, svelte_message, length);

            CallLocalFunction("OnPlayerSvelteAppMessage", "ddds", playerid, GetPlayerSvelteAppBrowserId(playerid), GetPlayerSvelteAppId(playerid), svelte_message);
        }
        else if(g_action_id == 24)
        {
            new index, bool:status;

			BS_ReadValue(bs,
				PR_UINT32, index,
				PR_BOOL, status
            );

            if(!status && IsPlayerConnected(playerid))
            {
                SendClientMessage(playerid, 0xFF6666FF, "(HookEscClick): GetPlayerSvelteAppId(playerid) = %d", GetPlayerSvelteAppId(playerid));

                if(GetPlayerSvelteAppId(playerid) > SVELTE_APP_NONE)
                {
                    if(!Svelte:GetPlayerAllowedEsc(playerid))
                    {
                        SetCursorBrowser(playerid, GetPlayerSvelteAppBrowserId(playerid), true);

                        SendClientMessage(playerid, 0xFF6666FF, "(HookEscClick): Вернули игроку курсор браузера.");
                    }
                    else
                        CallLocalFunction("OnPlayerClickEscSvelteApp", "ddd", playerid, GetPlayerSvelteAppId(playerid), GetPlayerSvelteAppBrowserId(playerid));
                }
                else
                {
                    for(new i; i < MAX_MODAL_TYPES; i ++)
                    {
                        if(!IsModalTypeOpened(playerid, i))
                            continue;

                        HidePlayerModal(playerid, i, .cursor = true);
                    }
                }
            }
        }     
    }
    return 1;
}
#if defined _ALS_OnIncomingPacket
    #undef OnIncomingPacket
#else
    #define _ALS_OnIncomingPacket
#endif
#define OnIncomingPacket browser_OnIncomingPacket
#if defined browser_OnIncomingPacket
    forward browser_OnIncomingPacket(playerid, packetid, BitStream:bs);
#endif