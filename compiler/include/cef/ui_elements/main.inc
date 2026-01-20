#if defined _cef_ui_inc
    #endinput
#endif
#define _cef_ui_inc

#define UIElement: ui_

enum
{
    UI_ELEMENT_NONE = -1,
    UI_COMMAND_BINDER = 1,
    UI_VOICE_SETTINGS = 2,
    UI_VOICE_PLAYERS_SETTINGS = 3,
    UI_DIALOG = 4,
    UI_RADIAL_MENU = 5,
    UI_SNACKBAR = 6,
    UI_ANIMATION_MENU = 7,
    UI_HUD = 8,
    UI_AUTHORIZATION = 9,
    UI_QUEST_DIALOG = 15,
    UI_PERSONAL_PROPERTY = 17,
    UI_METAL_DETECTOR = 21,
    UI_UNIVERSAL_ACTION = 25,
    UI_EXCAVATION_SCREEN = 28,
    UI_QUESTS_SCREEN = 29,
    UI_DICE_SCREEN = 33,
    UI_USER_BATTLE_PASS = 39,
    UI_MAIN_BATTLE_PASS = 40,
    UI_NEWBIE_BATTLE_PASS = 41,
    UI_PASS_PROMO = 38,
    UI_DONATE_SHOP = 42,
    UI_CONTAINER = 43,
    UI_CURRENT_CONTAINER = 44,
    UI_CONTAINER_REWARDS = 45,
    UI_ACTION = 46,
    UI_MOBILE_PHONE = 47,
    UI_GAS_STATION = 48,
    UI_CINEMA_EFFECT = 49,
    UI_NEW_QUEST = 50,
    UI_VEHICLE_PLATE = 51,
    UI_INVENTORY = 52,
    UI_INTERACTION_BUTTON = 53,
    UI_MAIN_MENU = 54,
    UI_STREET_FOOD = 55,
    UI_PLAYER_LIST = 56,
    UI_TRADE = 57,
    UI_CRAFT = 58,
    UI_WORKSHOP = 59,
    UI_GLOVO = 60,
    UI_LAVKA = 61,
    UI_INPUT_LAYOUT = 62,
    UI_STREAM_VIDEO = 63,
    UI_INVENTORY_SECURITY_SCREEN = 64,
    UI_INVENTORY_WAREHOUSE = 65,
    UI_INVENTORY_VEHICLE_SCREEN = 66,
    UI_INVENTORY_WALLET_SCREEN = 67,
    UI_PRODUCTS_SCREEN = 68,
    UI_PREVIEW_USER_INVENTORY = 72,
    UI_VIDEO_ADVICE = 73,
    UI_TUNING_SCREEN = 74,
    UI_EMPLOYMENT_TASK = 75,
    UI_CASE_ROULETTE = 76,
    UI_FLASH_GRENADE = 77,
    UI_SITUATION = 80,
    UI_FISHING = 81,
    UI_NEW_CONTAINER = 82,
    UI_BANNERS = 83,
    UI_BUSINESS = 84,
    UI_HINTS = 87,
    UI_DAILY_REWARD = 88,
    UI_LINKING = 89,
    UI_CARS = 91,
    UI_HOUSES = 94,
    UI_POTIONS = 95,
    UI_CASINO = 96,
    UI_REWARDS_SCREEN = 97,
    UI_BLUEPRINT = 98,
    UI_BATTLE_PASS = 99,
    UI_CATALOG = 101,
    UI_DOCUMENTS = 104,
    UI_EASTER_MENU = 110,
    UI_BP_EVENT_CHOICE = 111,
    UI_CONVEYOR_GAME = 112,
    MAX_UI_ELEMENTS
}

new g_player_ui_element[MAX_PLAYERS] = {UI_ELEMENT_NONE, ...};
new g_player_ui_hide_allowed[MAX_PLAYERS] = {false, ...};

stock UIElement:GetPlayerAllowedHide(playerid)
    return g_player_ui_hide_allowed[playerid];

stock GetPlayerUIElement(playerid)
    return g_player_ui_element[playerid];

stock SetUIElementVisibleEx(playerid, elementid, bool:toggle, const file[], line) {

	if(!IsMobileConnected(playerid) || !(0 < elementid < MAX_UI_ELEMENTS))
		return false;

    if(toggle)
        CallLocalFunction("OnPlayerShowUIElement", "dd", playerid, elementid);
    else
        CallLocalFunction("OnPlayerHideUIElement", "dd", playerid, elementid);

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 220,
		PR_UINT8, 62,
		PR_UINT8, elementid,
		PR_BOOL, toggle
    );

	printf("SetUIElementVisible(%d, %d, %b, %s, %d)", playerid, elementid, toggle, file, line);

	return PR_SendPacket(bs, playerid), BS_Delete(bs);
}
#define SetUIElementVisible(%0,%1,%2) SetUIElementVisibleEx(%0, %1, %2, __file, __line)

stock UIElement:SetPlayerAllowedHide(playerid, bool:enable)
{
    if(!IsMobileConnected(playerid))
        return false;

    SendClientMessage(playerid, -1, "UIElement:SetPlayerAllowedHide(playerid, = %d, enable = %b)", playerid, enable);

    return g_player_ui_hide_allowed[playerid] = enable;
}

stock UIElement:ExecuteEvent(playerid, elementid, subtype, encode, const message[])
{
	if(!IsMobileConnected(playerid))
		return false;

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 220,
		PR_UINT8, 84,
		PR_UINT8, elementid,
		PR_UINT8, subtype,
		PR_UINT16, strlen(message) + encode,
		PR_UINT8, encode,
		encode ? PR_CSTRING:PR_STRING, message
    );

	return PR_SendPacket(bs, playerid), BS_Delete(bs);
}

void OnPlayerMobileMessage(playerid, elementid, const type[], type_size, string[])
{
	if(!IsMobileConnected(playerid))
		return false;

    return true;
}

void OnPlayerShowUIElement(playerid, elementid)
{
    g_player_ui_element[playerid] = elementid;

    return true;
}

void OnPlayerHideUIElement(playerid, elementid)
{
    g_player_ui_element[playerid] = UI_ELEMENT_NONE;

    return true;
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

        if(g_action_id == 66)
        {
			new elementid, bool:toggle;

			BS_ReadValue(bs, \
				PR_UINT8, elementid,
				PR_BOOL, toggle
            );

            if(!UIElement:GetPlayerAllowedHide(playerid))
            {
                SetUIElementVisible(playerid, elementid, true);

                SendClientMessage(playerid, 0xFF0000FF, "(HookWebViewClose) UIElement:GetPlayerAllowedHide(playerid) = false");
            }
            else
                CallLocalFunction("OnPlayerHideUIElement", "dd", playerid, elementid);
        }
        else if(g_action_id == 63)
        {
			new elementid, clicked_values[2];
            new mobile_message[128];

			BS_ReadValue(bs,
				PR_UINT8, elementid,
				PR_UINT32, clicked_values[0],
				PR_UINT32, clicked_values[1]
            );

			new length;

			BS_ReadValue(bs, PR_UINT16, length);

            if(length <= 0 || length >= sizeof(mobile_message))
            {
                printf("[MobileMessage] Ошибка при чтении пакета (length < 0 || length > %d)", sizeof(mobile_message));

                return false;
            }
            else
                BS_ReadValue(bs, PR_STRING, mobile_message, length);

			CallLocalFunction("OnPlayerMobileMessage", "ddads", playerid, elementid, clicked_values, sizeof(clicked_values), mobile_message);
        }
    }
    return 1;
}
#if defined _ALS_OnIncomingPacket
    #undef OnIncomingPacket
#else
    #define _ALS_OnIncomingPacket
#endif
#define OnIncomingPacket ui_OnIncomingPacket
#if defined ui_OnIncomingPacket
    forward ui_OnIncomingPacket(playerid, packetid, BitStream:bs);
#endif