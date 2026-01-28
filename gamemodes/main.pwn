#define MIXED_SPELLINGS
#define NO_TAGS

#include <open.mp>

#undef MAX_PLAYERS
#define MAX_PLAYERS (2)

#include <Pawn.RakNet>
#include <Pawn.CMD>
#include <foreach>
#include <sscanf2>

main() {}

new mainString[4096];

#define void%0(%1) forward%0(%1); public%0(%1)

new g_player_name[MAX_PLAYERS][MAX_PLAYER_NAME];
#define Name(%0) g_player_name[%0]

stock GetXYInFrontOfPlayer(playerid, &Float:q, &Float:w, Float:distance)
{
    new Float:a;
    GetPlayerPos(playerid, q, w, a);
    GetPlayerFacingAngle(playerid, a);
    q += (distance * floatsin(-a, degrees));
    w += (distance * floatcos(-a, degrees));
    return 1;
}

stock IsNullFloat(const Float:check[], size = sizeof check)
{
	new Float:check_float_summ;

	for(new i; i < size; i++) check_float_summ += check[i];

	return (check_float_summ == 0.0);
}

stock IsAFireGun(gunid)
	return (gunid >= 22 && gunid <= 38);

stock Float:PointToPoint(Float:x,Float:y,Float:z,Float:x2,Float:y2,Float:z2)
	return floatsqroot(floatpower(floatabs(floatsub(x2,x)),2)+floatpower(floatabs(floatsub(y2,y)),2)+floatpower(floatabs(floatsub(z2,z)),2));

stock GetString(const param1[], const param2[], bool:param3 = false) return !strcmp(param1, param2, param3);

#define INTERIOR_NONE   (0)
#define WORLD_NONE      (0)

#include cef/main.inc
#include peds/main.inc

public OnGameModeInit()
{
    SetGameModeText("Test Server");

    print("started");

    CreateVehicle(411, 1765.5046, -1892.7008, 13.5611, 90.0, 255, 255, 150000);

    #if defined _cef_browser_inc
        Iter_Init(g_player_active_browsers);
    #endif

    #if defined _custom_peds_inc
        Iter_Init(PedStreamList);

        Iter_Clear(CustomPed);

        for(new i; i < MAX_SERVER_PEDS; i++) ClearCustomPedData(i);

        SetTimer("UpdateCustomPed", 500, true);

        CreateServerPed(18, 1765.5046, -1892.7008, 13.5611, 90.0, .name = "Голый негр");
    #endif

    return 1;
}

public OnGameModeExit()
{
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
    SetPlayerCameraPos(playerid, -1251.1089, 2551.7546, 104.6863);
    SetPlayerCameraLookAt(playerid, -1302.1554, 2533.4226, 93.8427);

    return 1;
}

public OnPlayerConnect(playerid)
{
    SendClientMessage(playerid, 0xFF0000FF, "Welcome, %s", Name(playerid));

    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid, 1765.5046, -1892.7008, 13.5611);
    SetPlayerFacingAngle(playerid, 175.9128);

    return 1;
}

CMD:debug(playerid)
{
    new app = GetPlayerSvelteAppId(playerid);

    if(app <= SVELTE_APP_NONE)
        SendClientMessage(playerid, 0x00FFFFFF, "GetPlayerSvelteAppId(playerid) = %d (null)", app);
    else
        SendClientMessage(playerid, 0x00FFFFFF, "GetPlayerSvelteAppId(playerid) = %d (%s)", app, g_svelte_app_names[app]);

    SendClientMessage(playerid, 0x00FFFFFF, "GetPlayerSvelteAppBrowserId(playerid) = %d", GetPlayerSvelteAppBrowserId(playerid));
    SendClientMessage(playerid, 0x00FFFFFF, "GetPlayerAllowedEsc(playerid) = %b", Svelte:GetPlayerAllowedEsc(playerid));

    SendClientMessage(playerid, 0x00FFFFFF, "IsLauncherConnected(playerid) = %b", IsLauncherConnected(playerid));
    SendClientMessage(playerid, 0x00FFFFFF, "IsMobileConnected(playerid) = %b", IsMobileConnected(playerid));
    SendClientMessage(playerid, 0x00FFFFFF, "IsTrilogyConnected(playerid) = %b", IsTrilogyConnected(playerid));

    return true;
}

CMD:donate(playerid)
{
    Svelte:SetPlayerAllowedEsc(playerid, true);

    return ShowPlayerSvelteApp(playerid, SVELTE_APP_DONATESHOP, true);
}

CMD:blockitem(playerid)
{
    SendMessageToFront(playerid, "event.donateShop.updateProduct", "{\"key\": 84,\"category\": 6,\"blockedType\": 1,\"blockedReason\": \"Товар уже приобретен\"}");
    SendMessageToFront(playerid, "event.donateShop.updateProduct", "{\"key\": 83,\"category\": 6,\"blockedType\": 1,\"blockedReason\": \"Товар уже приобретен\"}");
    SendMessageToFront(playerid, "event.donateShop.updateProduct", "{\"key\": 88,\"category\": 6,\"blockedType\": 1,\"blockedReason\": \"Товар уже приобретен\"}");
    SendMessageToFront(playerid, "event.donateShop.updateProduct", "{\"key\": 89,\"category\": 6,\"blockedType\": 1,\"blockedReason\": \"Товар уже приобретен\"}");

    return true;
}

CMD:numbers(playerid)
{
    Svelte:SetPlayerAllowedEsc(playerid, true);

    return ShowPlayerSvelteApp(playerid, SVELTE_APP_CARNUMBERS, true);
}

CMD:security(playerid)
{
    Svelte:SetPlayerAllowedEsc(playerid, true);

    return ShowPlayerModal(playerid, MODAL_TYPE_SECURITY_BANNER, true);
}

CMD:showdialogtip(playerid)
{
    PR_ShowDialogTip(playerid, .image = "quest_basic_background_11.webp", .text = "test message");

    return true;
}

CMD:hidedialogtip(playerid)
{
    HidePlayerModal(playerid, MODAL_TYPE_DIALOGTIP, false);

    return true;
}

#if defined _custom_dialog_inc

CMD:dialog(playerid, const params[])
{
    extract params -> new style; else
        return SendClientMessage(playerid, -1, "/dialog [style]");

    return CustomDialog:ShowPlayerDialog(playerid, 1234, style, "{FFFFFF}Это тестовое заглавие", "{FFFFFF}Ну, а это содержимое диалога", "Кнопочка 1", "Кнопочка 2");
}

#endif

stock PR_ShowDialogTip(playerid, const position[] = "rightBottom", const image[] = "", const icon[] = "icon-info", const icon_color[] = "#5FC6FF", const highlight_color[] = "#5FC6FF", const text[] = "")
{
	format(mainString, 1024, "\
	{\
		\"position\":\"%s\",\
		\"backgroundImage\":\"%s\",\
		\"icon\":\"%s\",\
		\"iconColor\":\"%s\",\
		\"highlightColor\":\"%s\",\
		\"text\":\"%s\"\
	}",

		position,
		image,
		icon,
		icon_color,
		highlight_color,
		text

	);

	return ShowPlayerModal(playerid, MODAL_TYPE_DIALOGTIP, false, mainString);
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    SendClientMessage(playerid, -1, "[OnDialogResponse]: dialogid = %d, response = %d, listitem = %d, inputtext = %s", dialogid, response, listitem, inputtext);

    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    g_player_name[playerid][0] = EOS;

    return 1;
}

#if defined _custom_peds_inc

cmd:addstream(playerid, const params[])
{
    extract params -> new pedid; else
        return false;

    return Iter_Add(PedStreamList[pedid], playerid);
}

cmd:createped(playerid)
{
    new Float:x, Float:y, Float:z, Float:angle;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);

    new pedid = CreateServerPed(18, x, y, z, angle, .name = "Голый негр");

    return SendClientMessage(playerid, -1, "CreateServerPed: pedid = %d", pedid);
}

cmd:destroyped(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/destroyped [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return DestroyServerPed(pedid, true);
}

cmd:pedhealth(playerid, const params[])
{
    extract params -> new pedid, Float:health; else
        return SendClientMessage(playerid, -1, "/pedhealth [pedid] [health]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return SetPedHealth(pedid, health);
}

cmd:pedarmour(playerid, const params[])
{
    extract params -> new pedid, Float:armour; else
        return SendClientMessage(playerid, -1, "/pedarmour [pedid] [armour]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return SetPedArmour(pedid, armour);
}

cmd:pedweapon(playerid, const params[])
{
    extract params -> new pedid, weapon, ammo; else
        return SendClientMessage(playerid, -1, "/pedarmour [pedid] [weapon] [ammo]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return GivePedWeapon(pedid, weapon, ammo);
}

cmd:pedpos(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/pedpos [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    return SetPedPos(pedid, x, y, z);
}

cmd:moveped(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/moveped [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    new Float:pos[3];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

    return MovePedToPos(pedid, .data = pos);
}

cmd:pedanim(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/pedanim [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return ApplyPedAnimation(pedid, "DANCING", "DAN_RIGHT_A", 80000);
}

cmd:pedattack(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/pedattack [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return AttackPedToPlayer(pedid, playerid);
}

cmd:stoppedattack(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/stoppedattack [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return StopPedAttack(pedid);
}

cmd:pedvehicle(playerid, const params[])
{
    extract params -> new pedid, vehicleid, seatid; else
        return SendClientMessage(playerid, -1, "/pedvehicle [pedid] [vehicleid] [seatid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    if(!IsValidVehicle(vehicleid))
        return SendClientMessage(playerid, -1, "!IsValidVehicle(vehicleid)");

    return PutPedInVehicle(pedid, vehicleid, seatid);
}

cmd:removepedvehicle(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/removepedvehicle [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return RemovePedFromVehicle(pedid);
}

cmd:pedchat(playerid, const params[])
{
    extract params -> new pedid; else
        return SendClientMessage(playerid, -1, "/pedchat [pedid]");

    if(!IsValidPed(pedid))
        return SendClientMessage(playerid, -1, "!IsValidPed(pedid)");

    return SetPedChatBubble(pedid, "test bubble", 0xFFFFFFFF, 10.0, 15000);
}

#endif