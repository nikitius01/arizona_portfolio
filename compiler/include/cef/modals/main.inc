#if defined _cef_modals_inc
    #endinput
#endif
#define _cef_modals_inc

#define Modal: modal_

enum
{
    MODAL_TYPE_NONE = -1,
    MODAL_TYPE_INFORMATION,
    MODAL_TYPE_BUSINESS_INFO,
    MODAL_TYPE_INTERACTION_SIDEBAR,
    MODAL_TYPE_DIALOG,
    MODAL_TYPE_KEYREACTION,
    MODAL_TYPE_DIALOGTIP,
    MODAL_TYPE_SECURITY_BANNER,
    MODAL_TYPE_GIVEAWAYPANEL,
    MODAL_TYPE_VEHICLE_MENU,
    MAX_MODAL_TYPES
};

new const g_modal_type_names[MAX_MODAL_TYPES][24] =
{
    "information", // MODAL_TYPE_INFORMATION
    "businessInfo", // MODAL_TYPE_BUSINESS_INFO
    "interactionSidebar", // MODAL_TYPE_INTERACTION_SIDEBAR
    "dialog", // MODAL_TYPE_DIALOG
    "keyReaction", // MODAL_TYPE_KEYREACTION
    "dialogTip", // MODAL_TYPE_DIALOGTIP
    "securityBanner", // MODAL_TYPE_SECURITY_BANNER
    "giveawayPanel", // MODAL_TYPE_GIVEAWAYPANEL
    "carMenu" // MODAL_TYPE_VEHICLE_MENU
};

new g_player_modal_type_opened[MAX_PLAYERS][MAX_MODAL_TYPES] = {{false, ...}, ...};

stock IsModalTypeOpened(playerid, modal)
    return g_player_modal_type_opened[playerid][modal];

new g_modal_string[256];

stock ShowPlayerModal(playerid, modal, bool:cursor, const data[] = "")
{
	if(!(MODAL_TYPE_NONE < modal < MAX_MODAL_TYPES))
		return SendClientMessage(playerid, -1, "HidePlayerModal: INVALID_MODAL_TYPE");

    SendClientMessage(playerid, -1, "ShowPlayerModal(playerid = %d, modal = %d, cursor = %b)", playerid, modal, cursor);

    format(g_modal_string, 256, "window.executeEvent('cef.modals.showModal', `[\"%s\"%s%s]`);", g_modal_type_names[modal], strlen(data) ? ",":"", data);
    SvelteApp_ExecuteEvent(playerid, 1, g_modal_string);

    if(cursor)
    {
        SetCursorBrowser(playerid, GetPlayerSvelteAppBrowserId(playerid), true);
    
        g_player_modal_type_opened[playerid][modal] = true;
    }

    CallLocalFunction("OnPlayerShowModal", "dd", playerid, modal);

    return true;
}

stock HidePlayerModal(playerid, modal, bool:cursor)
{
	if(!(MODAL_TYPE_NONE < modal < MAX_MODAL_TYPES))
		return SendClientMessage(playerid, -1, "HidePlayerModal: INVALID_MODAL_TYPE");

    SendClientMessage(playerid, -1, "HidePlayerModal(playerid = %d, modal = %d, cursor = %b)", playerid, modal, cursor);

    g_player_modal_type_opened[playerid][modal] = false;

    format(g_modal_string, 256, "window.executeEvent('cef.modals.closeModal', `[\"%s\"]`);", g_modal_type_names[modal]);
    SvelteApp_ExecuteEvent(playerid, 1, g_modal_string);

    if(cursor) SetCursorBrowser(playerid, GetPlayerSvelteAppBrowserId(playerid), !cursor);

    CallLocalFunction("OnPlayerHideModal", "dd", playerid, modal);

    return true;
}

#include cef/modals/custom_dialog/m_custom_dialog.inc

void OnPlayerShowModal(playerid, modal)
{
    SendClientMessage(playerid, 0xFF6666FF, "OnPlayerShowModal(%d, %d)", playerid, modal);

    return true;
}

void OnPlayerHideModal(playerid, modal)
{
    SendClientMessage(playerid, 0xFF6666FF, "OnPlayerHideModal(%d, %d)", playerid, modal);

    return true;
}