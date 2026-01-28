#if defined _custom_peds_inc
    #endinput
#endif
#define _custom_peds_inc

#define MAX_SERVER_PEDS (128)

#define INVALID_PED_ID  (-1)

enum custom_ped_data
{
	bool:customPedSpawned,
	customPedModel,
	Float:customPedPosition[4],
	customPedConnection,
	Float:customPedHealth[2],
	Float:customPedArmour[2],
	customPedWeapon[2],
	customPedNameColor,
	customPedName[MAX_PLAYER_NAME],
	customPedTextColor,
	customPedText[MAX_PLAYER_NAME * 2],

	/*Временные переменные*/
	customPedDeathStage,
	customPedAttackerID,
	Float:customPedTeleportPosition[3],
	Float:customPedMovingPosition[3],
	customPedAnimationLibrary[24],
	customPedAnimationName[24],
	customPedAnimationTime,
	customPedVehicle[2],
	bool:customPedVehicleEntered,

	/*Другое*/
	pedInterior,
	pedVirtualWorld,

	bool:pedAttackingPlayer
};
new CustomPedData[MAX_SERVER_PEDS][custom_ped_data];

new Iterator:CustomPed<MAX_SERVER_PEDS>;
new IteratorArray:PedStreamList[MAX_SERVER_PEDS]<MAX_PLAYERS>;

#include peds/utils.inc
#include peds/handlers.inc

stock IsValidPed(pedid)
    return Iter_Contains(CustomPed, pedid);

stock CreateServerPed
(
    model,
    Float:x, Float:y, Float:z, Float:angle,

    interiorid = INTERIOR_NONE,
    worldid = WORLD_NONE,

    Float:health = 100.0, Float:max_health = 100.0,
    Float:armour = 0.0, Float:max_armour = 100.0,

    weapon = 0, ammo = 0,

    const name[] = "", name_color = 0xFFFFFFFF,
    const text[] = "", text_color = 0xFFFFFFFF
) {
    new pedid = Iter_Free(CustomPed);

    if(pedid == ITER_NONE)
    {
        printf("[CreateServerPed] Не удалось создание педа, нету свободного места.");
        return INVALID_PED_ID;
    }

    CustomPedData[pedid][customPedModel] = model;

    CustomPedData[pedid][customPedPosition][0] = x;
    CustomPedData[pedid][customPedPosition][1] = y;
    CustomPedData[pedid][customPedPosition][2] = z;
    CustomPedData[pedid][customPedPosition][3] = angle;

    CustomPedData[pedid][pedInterior] = interiorid;
    CustomPedData[pedid][pedVirtualWorld] = worldid;

    CustomPedData[pedid][customPedHealth][0] = health;
    CustomPedData[pedid][customPedHealth][1] = max_health;

    CustomPedData[pedid][customPedArmour][0] = armour;
    CustomPedData[pedid][customPedArmour][1] = max_armour;

    CustomPedData[pedid][customPedWeapon][0] = weapon;
    CustomPedData[pedid][customPedWeapon][1] = ammo;

    CustomPedData[pedid][customPedNameColor] = name_color;
    strcopy(CustomPedData[pedid][customPedName], name);

    CustomPedData[pedid][customPedTextColor] = text_color;
    strcopy(CustomPedData[pedid][customPedText], text);

	CustomPedData[pedid][customPedConnection] = INVALID_PLAYER_ID;

    PedUtils:CreateServerPed(pedid, INVALID_PLAYER_ID);

	CustomPedData[pedid][customPedSpawned] = true;

    Iter_Add(CustomPed, pedid);

	printf("CreateServerPed(%d, %f, %f, %f)", model, x, y, z);

    return pedid;
}

stock DestroyServerPed(pedid, bool:iter_remove = false)
{
	if(!IsValidPed(pedid))
		return false;

	PedUtils:DestroyServerPed(pedid, INVALID_PLAYER_ID);

	ClearCustomPedData(pedid);

	if(iter_remove && Iter_Contains(CustomPed, pedid))
	{
		printf("[DestroyServerPed]: iter_remove && Iter_Contains(CustomPed, pedid = %d)", pedid);

		Iter_Remove(CustomPed, pedid);
	}
	return 1;
}

stock ClearCustomPedData(id)
{
	CustomPedData[id][customPedSpawned] = CustomPedData[id][customPedVehicleEntered] = CustomPedData[id][pedAttackingPlayer] = false;
	CustomPedData[id][customPedModel] = CustomPedData[id][customPedDeathStage] = 0;

	for(new i; i < 4; i++)
	{
		if(i < 2)
			CustomPedData[id][customPedHealth][i] = CustomPedData[id][customPedArmour][i] = CustomPedData[id][customPedWeapon][i] = CustomPedData[id][customPedVehicle][i] = 0;

		if(i < 3)
			CustomPedData[id][customPedTeleportPosition][i] = CustomPedData[id][customPedMovingPosition][i] = 0.0;

		CustomPedData[id][customPedPosition][i] = 0.0;
	}
	CustomPedData[id][customPedConnection] = CustomPedData[id][customPedAttackerID] = INVALID_PLAYER_ID;
	CustomPedData[id][customPedNameColor] = CustomPedData[id][customPedTextColor] = 0xffffff;

	strcopy(CustomPedData[id][customPedName], "");
	strcopy(CustomPedData[id][customPedText], "");

	strcopy(CustomPedData[id][customPedAnimationLibrary], "");
	strcopy(CustomPedData[id][customPedAnimationName], "");

	CustomPedData[id][customPedAnimationTime] = 0;

	Iter_Clear(PedStreamList[id]);
}

stock SetPedHealth(pedid, Float:amount, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    CustomPedData[pedid][customPedHealth][0] = amount;

    return PedUtils:UpdateOnFootPedSync(pedid, false, forplayerid);
}

stock SetPedArmour(pedid, Float:amount, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    CustomPedData[pedid][customPedArmour][0] = amount;

    return PedUtils:UpdateOnFootPedSync(pedid, false, forplayerid);
}

stock GivePedWeapon(pedid, weapon, ammo, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    CustomPedData[pedid][customPedWeapon][0] = weapon;
    CustomPedData[pedid][customPedWeapon][1] = ammo;

    return PedUtils:UpdatePedWeapon(pedid, forplayerid);
}

stock SetPedPos(pedid, Float:x, Float:y, Float:z, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    CustomPedData[pedid][customPedTeleportPosition][0] = x;
    CustomPedData[pedid][customPedTeleportPosition][1] = y;
    CustomPedData[pedid][customPedTeleportPosition][2] = z;

    return PedUtils:SetPedPos(pedid, forplayerid);
}

stock MovePedToPos(index, playerid = INVALID_PLAYER_ID, const Float:data[] = {0.0, 0.0, 0.0}, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(index))
		return false;

	new check_ped_stream_iter_count = Iter_Count(PedStreamList[index]);
	if(playerid != INVALID_PLAYER_ID)
	{
		GetPlayerPos(playerid, CustomPedData[index][customPedMovingPosition][0], CustomPedData[index][customPedMovingPosition][1], CustomPedData[index][customPedMovingPosition][2]);

		GetXYInFrontOfPlayer(playerid, CustomPedData[index][customPedMovingPosition][0], CustomPedData[index][customPedMovingPosition][1], -1.5);
	}
	else
	{
		if(check_ped_stream_iter_count > 0)
		{
			for(new i; i < 3; i++)
				CustomPedData[index][customPedMovingPosition][i] = data[i];
		}
		else
		{
			for(new i; i < 3; i++)
				CustomPedData[index][customPedMovingPosition][i] = 0.0,
				CustomPedData[index][customPedPosition][i] = data[i];

			return 1;
		}
	}

	PedUtils:MovePedToPos(index, forplayerid);

	return 1;
}

stock ApplyPedAnimation(pedid, const anim_lib[], const anim_name[], time, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    strcopy(CustomPedData[pedid][customPedAnimationLibrary], anim_lib);
    strcopy(CustomPedData[pedid][customPedAnimationName], anim_name);

    CustomPedData[pedid][customPedAnimationTime] = time;

    return PedUtils:ApplyPedAnimation(pedid, forplayerid);
}

stock AttackPedToPlayer(pedid, playerid, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    else if(!IsPlayerConnected(playerid))
        return false;

    CustomPedData[pedid][customPedAttackerID] = playerid;

    return PedUtils:AttackPlayer(pedid, forplayerid);
}

stock PedShootAtPlayer(pedid, playerid, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    else if(!IsPlayerConnected(playerid))
        return false;

    CustomPedData[pedid][customPedAttackerID] = playerid;

    return PedUtils:ShootAtPlayer(pedid, forplayerid);
}

stock StopPedAttack(pedid, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

	CustomPedData[pedid][customPedAttackerID] = INVALID_PLAYER_ID;

    return PedUtils:StopPlayerAttack(pedid, forplayerid);
}

stock PutPedInVehicle(pedid, vehicleid, seatid, time = 1000, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    else if(!IsValidVehicle(vehicleid))
        return false;

    CustomPedData[pedid][customPedVehicle][0] = vehicleid;
    CustomPedData[pedid][customPedVehicle][1] = seatid;

    return PedUtils:PutPedInVehicle(pedid, forplayerid, time);
}

stock RemovePedFromVehicle(pedid, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    return PedUtils:RemovePedFromVehicle(pedid, forplayerid);
}

stock SetPedChatBubble(pedid, const text[], colour, Float:drawDistance, expireTime, forplayerid = INVALID_PLAYER_ID)
{
	if(!IsValidPed(pedid))
		return false;

    return PedUtils:SetPedChatBubble(pedid, text, colour, drawDistance, expireTime, forplayerid);
}

stock SetCustomPedConnection(pedid, playerid)
{
	if(!IsValidPed(pedid))
		return false;

	else if(!IsPlayerConnected(playerid))
		return false;

	return CustomPedData[pedid][customPedConnection] = playerid;
}

void OnPedGiveDamage(pedid, damagedid, Float:amount, weaponid, bodypart)
{
	SendClientMessageToAll(0x6495EDFF, "OnPedGiveDamage(%d, %d, %.2f, %d, %d)", pedid, damagedid, amount, weaponid, bodypart);

	return 1;
}

void OnPedTakeDamage(pedid, issuerid, Float:amount, weaponid, bodypart)
{
	SendClientMessageToAll(0x6495EDFF, "OnPedTakeDamage(%d, %d, %.2f, %d, %d)", pedid, issuerid, amount, weaponid, bodypart);

	return 1;
}

void OnPedDeath(pedid, killerid, reason)
{
	SendClientMessageToAll(0x6495EDFF, "OnPedDeath(%d, %d, %d)", pedid, killerid, reason);

	return 1;
}

void OnPedStreamIn(playerid, pedid)
{
	SendClientMessage(playerid, 0x6495EDFF, "OnPedStreamIn(%d, %d)", playerid, pedid);
	
	return 1;
}

void OnPedStreamOut(playerid, pedid)
{
	SendClientMessage(playerid, 0x6495EDFF, "OnPedStreamOut(%d, %d)", playerid, pedid);

	return 1;
}