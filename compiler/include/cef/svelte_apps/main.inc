#if defined _cef_svelte_inc
    #endinput
#endif
#define _cef_svelte_inc

#define Svelte: SvelteApp_

enum
{
    SVELTE_APP_NONE = -1,
    SVELTE_APP_INTERACTIONMENU,
    SVELTE_APP_BATTLEROYALE,
    SVELTE_APP_BATTLEROYALEINVENTORY,
    SVELTE_APP_BATTLEROYALEHUD,
    SVELTE_APP_BATTLEROYALEMAP,
    SVELTE_APP_ARIZONAPASS,
    SVELTE_APP_DONATIONSHOP,
    SVELTE_APP_CONTAINERSAUCTION,
    SVELTE_APP_FADESCREEN,
    SVELTE_APP_FILMEDTEXT,
    SVELTE_APP_ACTIONPROGRESSBAR,
    SVELTE_APP_PHONE,
    SVELTE_APP_GASSTATION,
    SVELTE_APP_AUTH,
    SVELTE_APP_VIDEOBACKGROUND,
    SVELTE_APP_ANIMATIONSMENU,
    SVELTE_APP_SCREAMER,
    SVELTE_APP_ACTIONBAR,
    SVELTE_APP_STREETFOOD,
    SVELTE_APP_NPCDIALOG,
    SVELTE_APP_HORIZONTALPHONE,
    SVELTE_APP_QUESTS,
    SVELTE_APP_DICE,
    SVELTE_APP_CARNUMBERS,
    SVELTE_APP_CLICKER,
    SVELTE_APP_EMPLOYMENTHISTORY,
    SVELTE_APP_CRATEROULETTE,
    SVELTE_APP_TUNINGSTATION,
    SVELTE_APP_BUSINESSESLIST,
    SVELTE_APP_BUSINESSINFORMATION,
    SVELTE_APP_SITUATIONS,
    SVELTE_APP_CONTAINERCONTENT,
    SVELTE_APP_INVENTORY,
    SVELTE_APP_PROPERTYLIST,
    SVELTE_APP_PROPERTYINFORMATION,
    SVELTE_APP_CASINOROULETTE,
    SVELTE_APP_CASINOSLOTSMACHINE,
    SVELTE_APP_REWARDBANNER,
    SVELTE_APP_MOUNTAINTESTDRIVE,
    SVELTE_APP_TRUCKERS,
    SVELTE_APP_GLOBALEVENTMENU,
    SVELTE_APP_GLOBALEVENTQUESTS,
    SVELTE_APP_REWARDSNEWYEAR,
    SVELTE_APP_BLUEPRINT,
    SVELTE_APP_INTERACTIVEMENU,
    SVELTE_APP_DOCUMENTS,
    SVELTE_APP_FINDGAME,
    SVELTE_APP_BATTLEPASSCHOSE,
    SVELTE_APP_EVENTPASS,
    SVELTE_APP_METALDETECTOR,
    SVELTE_APP_EXCAVATIONS,
    SVELTE_APP_FAMILY,
    SVELTE_APP_DONATESHOP,
    SVELTE_APP_EGGSSORTGAME,
    SVELTE_APP_DOCUMENTSSORTGAME,
    SVELTE_APP_GHETTOMAP,
    SVELTE_APP_CRYSTALSSORTGAME,
    MAX_SVELTE_APPS
};

new const g_svelte_app_names[MAX_SVELTE_APPS][24] =
{
    "InteractionMenu", // SVELTE_APP_INTERACTIONMENU
    "BattleRoyale", // SVELTE_APP_BATTLEROYALE
    "BattleRoyaleInventory", // SVELTE_APP_BATTLEROYALEINVENTORY
    "BattleRoyaleHud", // SVELTE_APP_BATTLEROYALEHUD
    "BattleRoyaleMap", // SVELTE_APP_BATTLEROYALEMAP
    "ArizonaPass", // SVELTE_APP_ARIZONAPASS
    "DonationShop", // SVELTE_APP_DONATIONSHOP
    "ContainersAuction", // SVELTE_APP_CONTAINERSAUCTION
    "FadeScreen", // SVELTE_APP_FADESCREEN
    "FilmedText", // SVELTE_APP_FILMEDTEXT
    "ActionProgressBar", // SVELTE_APP_ACTIONPROGRESSBAR
    "Phone", // SVELTE_APP_PHONE
    "GasStation", // SVELTE_APP_GASSTATION
    "Auth", // SVELTE_APP_AUTH
    "VideoBackground", // SVELTE_APP_VIDEOBACKGROUND
    "AnimationsMenu", // SVELTE_APP_ANIMATIONSMENU
    "Screamer", // SVELTE_APP_SCREAMER
    "ActionBar", // SVELTE_APP_ACTIONBAR
    "StreetFood", // SVELTE_APP_STREETFOOD
    "NpcDialog", // SVELTE_APP_NPCDIALOG
    "HorizontalPhone", // SVELTE_APP_HORIZONTALPHONE
    "Quests", // SVELTE_APP_QUESTS
    "Dice", // SVELTE_APP_DICE
    "CarNumbers", // SVELTE_APP_CARNUMBERS
    "Clicker", // SVELTE_APP_CLICKER
    "EmploymentHistory", // SVELTE_APP_EMPLOYMENTHISTORY
    "CrateRoulette", // SVELTE_APP_CRATEROULETTE
    "TuningStation", // SVELTE_APP_TUNINGSTATION
    "BusinessesList", // SVELTE_APP_BUSINESSESLIST
    "BusinessInformation", // SVELTE_APP_BUSINESSINFORMATION
    "Situations", // SVELTE_APP_SITUATIONS
    "ContainerContent", // SVELTE_APP_CONTAINERCONTENT
    "Inventory", // SVELTE_APP_INVENTORY
    "PropertyList", // SVELTE_APP_PROPERTYLIST
    "PropertyInformation", // SVELTE_APP_PROPERTYINFORMATION
    "CasinoRoulette", // SVELTE_APP_CASINOROULETTE
    "CasinoSlotsMachine", // SVELTE_APP_CASINOSLOTSMACHINE
    "RewardBanner", // SVELTE_APP_REWARDBANNER
    "MountainTestDrive", // SVELTE_APP_MOUNTAINTESTDRIVE
    "Truckers", // SVELTE_APP_TRUCKERS
    "GlobalEventMenu", // SVELTE_APP_GLOBALEVENTMENU
    "GlobalEventQuests", // SVELTE_APP_GLOBALEVENTQUESTS
    "RewardsNewYear", // SVELTE_APP_REWARDSNEWYEAR
    "Blueprint", // SVELTE_APP_BLUEPRINT
    "InteractiveMenu", // SVELTE_APP_INTERACTIVEMENU
    "Documents", // SVELTE_APP_DOCUMENTS
    "FindGame", // SVELTE_APP_FINDGAME
    "BattlePassChose", // SVELTE_APP_BATTLEPASSCHOSE
    "EventPass", // SVELTE_APP_EVENTPASS
    "MetalDetector", // SVELTE_APP_METALDETECTOR
    "Excavations", // SVELTE_APP_EXCAVATIONS
    "Family", // SVELTE_APP_FAMILY
    "DonateShop", // SVELTE_APP_DONATESHOP
    "EggsSortGame", // SVELTE_APP_EGGSSORTGAME
    "DocumentsSortGame", // SVELTE_APP_DOCUMENTSSORTGAME
    "GhettoMap", // SVELTE_APP_GHETTOMAP
    "CrystalsSortGame" // SVELTE_APP_CRYSTALSSORTGAME
};

new g_player_svelte_app[MAX_PLAYERS] = {SVELTE_APP_NONE, ...};
new g_player_svelte_app_browser[MAX_PLAYERS] = {-1, ...};

new bool:g_player_svelte_allowed_esc[MAX_PLAYERS] = {false, ...};

new g_svelte_string[256];

stock GetPlayerSvelteAppId(playerid)
    return g_player_svelte_app[playerid];

stock GetPlayerSvelteAppBrowserId(playerid)
    return g_player_svelte_app_browser[playerid];

stock SetPlayerSvelteAppBrowserId(playerid, index)
{
    SendClientMessage(playerid, 0xFF0000FF, "SetPlayerSvelteAppBrowserId(playerid = %d, index = %d)", playerid, index);

    return g_player_svelte_app_browser[playerid] = index;
}

stock Svelte:GetPlayerAllowedEsc(playerid)
    return g_player_svelte_allowed_esc[playerid];

stock ShowPlayerSvelteApp(playerid, app, bool:cursor)
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return false;

    else if(!(SVELTE_APP_NONE < app < MAX_SVELTE_APPS))
        return false;

    new browserid = GetPlayerSvelteAppBrowserId(playerid);

    SendClientMessage(playerid, -1, "ShowPlayerSvelteApp(playerid = %d, app = %d, cursor = %b): GetPlayerSvelteAppBrowserId(playerid) = %d", playerid, app, cursor, browserid);

    g_player_svelte_app[playerid] = app;

    format(g_svelte_string, 256, "window.executeEvent('event.setActiveView', `[\"%s\"]`);", g_svelte_app_names[app]);
    Svelte:ExecuteEvent(playerid, 0, g_svelte_string);

    if(cursor) SetCursorBrowser(playerid, browserid, true);

    CallLocalFunction("OnPlayerShowSvelteApp", "ddds", playerid, app, browserid, g_svelte_app_names[app]);

    return true;
}

stock HidePlayerSvelteApp(playerid, bool:cursor)
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return false;

    SendClientMessage(playerid, -1, "HidePlayerSvelteApp(playerid, = %d, cursor = %b)", playerid, cursor);

    if(cursor) SetCursorBrowser(playerid, GetPlayerSvelteAppBrowserId(playerid), !cursor);

    CallLocalFunction("OnPlayerHideSvelteApp", "ddb", playerid, GetPlayerSvelteAppId(playerid), cursor);

    g_player_svelte_app[playerid] = SVELTE_APP_NONE;

    Svelte:ExecuteEvent(playerid, 0, "window.executeEvent('event.setActiveView', '[ null ]');");

    return true;
}

stock Svelte:SetPlayerAllowedEsc(playerid, bool:enable)
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return false;

    SendClientMessage(playerid, -1, "Svelte:SetPlayerAllowedEsc(playerid, = %d, enable = %b)", playerid, enable);

    return g_player_svelte_allowed_esc[playerid] = enable;
}

stock Svelte:ExecuteEvent(playerid, encode, const event[])
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return false;

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 220,
		PR_UINT8, 17,
		PR_UINT32, GetPlayerSvelteAppBrowserId(playerid),
		PR_UINT16, strlen(event) + encode,
		PR_UINT8, encode,
		encode ? PR_CSTRING:PR_STRING, event,
		PR_UINT32, GetPlayerSvelteAppId(playerid)
    );

	return PR_SendPacket(bs, playerid), BS_Delete(bs);
}

void OnPlayerShowSvelteApp(playerid, app, index, const app_name[])
{
    SendClientMessage(playerid, 0xADFF2FFF, "OnPlayerShowSvelteApp(%d, %d, %d, %s)", playerid, app, index, app_name);

    return true;
}

void OnPlayerHideSvelteApp(playerid, app, cursor)
{
    SendClientMessage(playerid, 0xFFD700FF, "OnPlayerHideSvelteApp(%d, %d, %b)", playerid, app, cursor);

    return true;
}

void OnPlayerSvelteAppMessage(playerid, index, app, const message[])
{
    SendClientMessage(playerid, 0xFF00FFFF, "OnPlayerSvelteAppMessage(%d, %d, %d, %s)", playerid, app, index, message);

    if (strfind(message, "sendResponse") != -1) 
    {
        if(!IsModalTypeOpened(playerid, MODAL_TYPE_DIALOG))
            return false;

        new dialogid, response, listitem, inputtext[256];

        sscanf(message, "p<|>{s[32]}ddds[256]", dialogid, listitem, response, inputtext);

        HidePlayerModal(playerid, MODAL_TYPE_DIALOG, true);

        if(!IsPlayerControllable(playerid)) TogglePlayerControllable(playerid, true);

        CallLocalFunction("OnDialogResponse", "dddds", playerid, dialogid, response, listitem, inputtext);
    }

    return true;
}

void OnPlayerClickEscSvelteApp(playerid, app, index)
{
    SendClientMessage(playerid, 0x0000FFFF, "OnPlayerClickEscSvelteApp(%d, %d, %d)", playerid, app, index);

    HidePlayerSvelteApp(playerid, .cursor = true);

    return true;
}