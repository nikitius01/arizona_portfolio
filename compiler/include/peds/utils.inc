#define PedUtils: ped_utils_

stock PedUtils:CreateServerPed(pedid, forplayerid)
{
    new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 50,
		PR_UINT16, pedid,
        PR_UINT16, CustomPedData[pedid][customPedModel],
        PR_FLOAT4, CustomPedData[pedid][customPedPosition],
        PR_BOOL, false,
        PR_FLOAT, CustomPedData[pedid][customPedHealth][0],
        PR_FLOAT, CustomPedData[pedid][customPedArmour][0],
        PR_UINT16, CustomPedData[pedid][customPedWeapon][0],
        PR_UINT8, CustomPedData[pedid][customPedWeapon][1],
        PR_UINT32, CustomPedData[pedid][customPedNameColor],
        PR_STRING32, CustomPedData[pedid][customPedName],
        PR_UINT8, 0,
        PR_UINT32, CustomPedData[pedid][customPedTextColor],
        PR_STRING32, CustomPedData[pedid][customPedText],
        PR_UINT16, 0,
        PR_UINT16, INVALID_PLAYER_ID
    );

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:DestroyServerPed(pedid, forplayerid)
{
    new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 51,
		PR_UINT16, pedid
	);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:UpdateOnFootPedSync(pedid, bool:damage_update, forplayerid)
{
	if(CustomPedData[pedid][customPedHealth][0] < 0.0) CustomPedData[pedid][customPedHealth][0] = 0.0;
	if(CustomPedData[pedid][customPedArmour][0] < 0.0) CustomPedData[pedid][customPedArmour][0] = 0.0;

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 52,
		PR_UINT16, pedid,
		PR_UINT32, 0,
		PR_BOOL, damage_update,
        PR_FLOAT, CustomPedData[pedid][customPedHealth][0],
        PR_FLOAT, CustomPedData[pedid][customPedHealth][1] /*Макс. ХП*/,
        PR_FLOAT, CustomPedData[pedid][customPedArmour][0],
        PR_FLOAT, CustomPedData[pedid][customPedArmour][1] /*Макс. Брони*/
    );

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:UpdateInCarPedSync(pedid, forplayerid)
{
	if(CustomPedData[pedid][customPedHealth][0] < 0.0) CustomPedData[pedid][customPedHealth][0] = 0.0;
	if(CustomPedData[pedid][customPedArmour][0] < 0.0) CustomPedData[pedid][customPedArmour][0] = 0.0;

	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 52,
		PR_UINT16, pedid,
		PR_UINT16, CustomPedData[pedid][customPedVehicle][0],
		PR_UINT16, CustomPedData[pedid][customPedVehicle][1],
		PR_FLOAT, CustomPedData[pedid][customPedHealth][0],
		PR_FLOAT, CustomPedData[pedid][customPedArmour][0]
    );

	GetVehiclePos
	(
		CustomPedData[pedid][customPedVehicle][0],
		CustomPedData[pedid][customPedPosition][0],
		CustomPedData[pedid][customPedPosition][1],
		CustomPedData[pedid][customPedPosition][2]
	);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:UpdatePedWeapon(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 65,
		PR_UINT16, pedid,
        PR_UINT16, CustomPedData[pedid][customPedWeapon][0],
        PR_UINT8, CustomPedData[pedid][customPedWeapon][1]
    );

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:SetPedPos(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 66,
		PR_UINT16, pedid,
		PR_FLOAT3, CustomPedData[pedid][customPedTeleportPosition]
	);

	for(new i; i < 3; i++)
		CustomPedData[pedid][customPedPosition][i] = CustomPedData[pedid][customPedTeleportPosition][i],
		CustomPedData[pedid][customPedTeleportPosition][i] = 0.0;

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:MovePedToPos(pedid, forplayerid, time = 10000)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 67,
		PR_UINT16, pedid,
		PR_FLOAT3, CustomPedData[pedid][customPedMovingPosition],
		PR_UINT16, 7,
		PR_UINT32, time
	);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:ApplyPedAnimation(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 69,
		PR_UINT16, pedid,
		PR_STRING32, CustomPedData[pedid][customPedAnimationLibrary],
		PR_STRING32, CustomPedData[pedid][customPedAnimationName],
		PR_UINT32, CustomPedData[pedid][customPedAnimationTime]
	);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:ShootAtPlayer(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 72,
		PR_UINT16, pedid,
		PR_UINT16, CustomPedData[pedid][customPedAttackerID]
	);

	CustomPedData[pedid][pedAttackingPlayer] = true;

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:AttackPlayer(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 80,
		PR_UINT16, pedid,
		PR_UINT16, CustomPedData[pedid][customPedAttackerID],
		PR_UINT16, INVALID_PLAYER_ID,
		PR_UINT16, INVALID_PLAYER_ID
	);

	CustomPedData[pedid][pedAttackingPlayer] = true;

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:PutPedInVehicle(pedid, forplayerid, time = 1000)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 81,
		PR_UINT16, pedid,
		PR_UINT16, CustomPedData[pedid][customPedVehicle][0],
		PR_UINT16, CustomPedData[pedid][customPedVehicle][1],
		PR_UINT32, time
    );

	SetTimerEx("SendNPCEnterVehicleFunction", time, false, "d", pedid);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:RemovePedFromVehicle(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 84,
		PR_UINT16, pedid
    );

	for(new i; i < 2; i++)
		CustomPedData[pedid][customPedVehicle][i] = 0;

	SendNPCEnterVehicleFunction(pedid);

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:SetPedChatBubble(pedid, const text[], colour, Float:drawDistance, expireTime, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 85,
		PR_UINT16, pedid,
		PR_STRING32, text,
		PR_UINT32, colour,
		PR_FLOAT, drawDistance,
		PR_UINT32, expireTime
    );

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

stock PedUtils:StopPlayerAttack(pedid, forplayerid)
{
	new BitStream:bs = BS_New();

	BS_WriteValue(bs,
		PR_UINT8, 221,
		PR_UINT16, 102,
		PR_UINT16, pedid,
		PR_BOOL, true
    );

	CustomPedData[pedid][pedAttackingPlayer] = false;

	SendPedSync(bs, forplayerid, pedid);

	return BS_Delete(bs);
}

void SendNPCEnterVehicleFunction(id) CustomPedData[id][customPedVehicleEntered] = !CustomPedData[id][customPedVehicle][0] ? false:true;

stock SendPedSync(BitStream:bs, forplayerid, pedid)
{
	if(forplayerid == INVALID_PLAYER_ID)
	{
		foreach(PedStreamList[pedid], playerid)
			PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 2);
	}
	else
		PR_SendPacket(bs, forplayerid, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 2);

	return true;
}