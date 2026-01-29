#define PedHandler ped_handler_

IPacket:221(playerid, BitStream:bs)
{
	if(!IsPlayerConnected(playerid))
		return false;

	BS_SetReadOffset(bs, 8);

	new functionid;

	BS_ReadUint16(bs, functionid);

	switch(functionid)
	{
		case 53:
		{
			new id;

			BS_ReadUint16(bs, id);

			if(!Iter_Contains(CustomPed, id))
				return false;

			else if(!Iter_Contains(PedStreamList[id], playerid))
				return false;

			BS_ReadFloat3(bs, CustomPedData[id][customPedPosition]);

			if(!IsNullFloat(CustomPedData[id][customPedMovingPosition]))
			{
				new Float:moving_distance_from_pos = PointToPoint(
					CustomPedData[id][customPedPosition][0], CustomPedData[id][customPedPosition][1], CustomPedData[id][customPedPosition][2],
					CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], CustomPedData[id][customPedMovingPosition][2]
				);
				if(moving_distance_from_pos < 1.5)
					for(new i; i < 3; i++)
						CustomPedData[id][customPedMovingPosition][i] = 0.0;
			}
		}
		case 73:
		{
			new bool:hit_or_take, harm_id, Float:damage_amount, weapon_id, body_id, damager_id;
	
			BS_ReadValue(bs,
				PR_BOOL, hit_or_take,
				PR_UINT16, harm_id,
				PR_FLOAT, damage_amount,
				PR_UINT32, weapon_id,
				PR_UINT32, body_id,
				PR_IGNORE_BITS, 16,
				PR_UINT16, damager_id
			);

			if(!Iter_Contains(CustomPed, harm_id))
				return false;

			else if(!Iter_Contains(PedStreamList[harm_id], damager_id))
				return false;

			new pedPlayerConnection = CustomPedData[harm_id][customPedConnection];

			switch(hit_or_take)
            {
                case false:
                {
					if(Iter_Contains(PedStreamList[harm_id], damager_id))
					{
						CallLocalFunction("OnPedGiveDamage", "ddfdd", harm_id, damager_id, damage_amount, weapon_id, body_id);
					}
                }
                case true:
                {
					if(pedPlayerConnection != INVALID_PLAYER_ID && pedPlayerConnection != damager_id) {
						CustomPedData[harm_id][customPedAttackerID] = damager_id;
					}

					while(damage_amount > 0.0)
					{
						if(CustomPedData[harm_id][customPedArmour][0] > 0)
						{
							new Float:damage = damage_amount >= CustomPedData[harm_id][customPedArmour][0] ? CustomPedData[harm_id][customPedArmour][0]:damage_amount;

							CustomPedData[harm_id][customPedArmour][0] -= damage, damage_amount -= damage;
						}
						else
						{
							if(CustomPedData[harm_id][customPedHealth][0] <= 0.0)
								break;

							new Float:damage = damage_amount >= CustomPedData[harm_id][customPedHealth][0] ? CustomPedData[harm_id][customPedHealth][0]:damage_amount;

							CustomPedData[harm_id][customPedHealth][0] -= damage, damage_amount -= damage;
						}
						if(damage_amount <= 0.0) break;
					}
					PedUtils:UpdateOnFootPedSync(harm_id, false, INVALID_PLAYER_ID);

					CallLocalFunction("OnPedTakeDamage", "ddfdd", harm_id, damager_id, damage_amount, weapon_id, body_id);

					if(CustomPedData[harm_id][customPedHealth][0] <= 0.0 && CustomPedData[harm_id][customPedDeathStage] <= 0)
					{
						CallLocalFunction("OnPedDeath", "ddd", harm_id, damager_id, weapon_id);
					}
                }
            }
		}
	}
	return true;
}

void UpdateCustomPed()
{
	foreach(CustomPed, id)
	{
		if(!Iter_Contains(CustomPed, id))
		{
			printf("[UpdateCustomPed: %s(%d)] (!Iter_Contains(CustomPed, id)) id: %d", __file, __line, id);
			continue;
		}
		foreach(Player, playerid)
		{
			if(GetPlayerInterior(playerid) == CustomPedData[id][pedInterior] && GetPlayerVirtualWorld(playerid) == CustomPedData[id][pedVirtualWorld] && IsPlayerInRangeOfPoint(playerid, 100.0, CustomPedData[id][customPedPosition][0], CustomPedData[id][customPedPosition][1], CustomPedData[id][customPedPosition][2]))
			{
				if(Iter_Contains(PedStreamList[id], playerid))
					continue;

				Iter_Add(PedStreamList[id], playerid);

				PedUtils:CreateServerPed(id, playerid);

				if(!IsNullFloat(CustomPedData[id][customPedMovingPosition])) MovePedToPos(id, INVALID_PLAYER_ID, CustomPedData[id][customPedMovingPosition], playerid);

				if(CustomPedData[id][pedAttackingPlayer] && CustomPedData[id][customPedAttackerID] != INVALID_PLAYER_ID)
				{
					if(IsAFireGun(CustomPedData[id][customPedWeapon][0]))
						PedUtils:ShootAtPlayer(id, playerid);
					else
						PedUtils:AttackPlayer(id, playerid);
				}

				CallLocalFunction("OnPedStreamIn", "dd", playerid, id);
			}
			else if(Iter_Contains(PedStreamList[id], playerid))
			{
				Iter_Remove(PedStreamList[id], playerid);

				PedUtils:DestroyServerPed(id, playerid);

				CallLocalFunction("OnPedStreamOut", "dd", playerid, id);
			}
		}
		new currentID = id, pedConnection = CustomPedData[id][customPedConnection];
		if(CustomPedData[id][customPedHealth][0] > 0.0)
		{
			new Float:player_distance, Float:player_moving_distance, Float:check_valid_attacker_distance[2];

			if(pedConnection != INVALID_PLAYER_ID)
			{
				check_valid_attacker_distance[0] = 20.0;
				check_valid_attacker_distance[1] = 40.0;
			}

			if(CustomPedData[id][customPedAttackerID] != INVALID_PLAYER_ID)
			{
				new bool:not_valid_attacker_id = (!IsPlayerConnected(CustomPedData[id][customPedAttackerID]) || !Iter_Contains(PedStreamList[id], CustomPedData[id][customPedAttackerID]));

                if(not_valid_attacker_id != true)
                {

                }

				if(not_valid_attacker_id)
				{
					PedUtils:StopPlayerAttack(id, INVALID_PLAYER_ID);

					CustomPedData[id][customPedAttackerID] = INVALID_PLAYER_ID;

					if(pedConnection != INVALID_PLAYER_ID) MovePedToPos(id, pedConnection);
                }
				else
				{
					player_distance = GetPlayerDistanceFromPoint(CustomPedData[id][customPedAttackerID], CustomPedData[id][customPedPosition][0], CustomPedData[id][customPedPosition][1], CustomPedData[id][customPedPosition][2]);
					player_moving_distance = GetPlayerDistanceFromPoint(CustomPedData[id][customPedAttackerID], CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], CustomPedData[id][customPedMovingPosition][2]);

					if(!IsNullFloat(CustomPedData[id][customPedMovingPosition]))
					{
						if(player_moving_distance > 2.0)
						{
							PedUtils:StopPlayerAttack(id, INVALID_PLAYER_ID);

							MovePedToPos(id, CustomPedData[id][customPedAttackerID]);

							SendClientMessageToAll(0xFF0000FF, "MovePedToPos(index: %d) (бежать на новую координату, потому что дистанция игрока сместилась на 2 метра)", id);
						}
					}
					if(player_distance < check_valid_attacker_distance[1])
					{
						if(player_distance < check_valid_attacker_distance[0])
						{
							if(CustomPedData[id][pedAttackingPlayer] != true)
							{
								if(IsAFireGun(CustomPedData[id][customPedWeapon][0]))
									PedUtils:ShootAtPlayer(id, INVALID_PLAYER_ID);
								else
									PedUtils:AttackPlayer(id, INVALID_PLAYER_ID);

								SendClientMessageToAll(0xFF0000FF, "ShootAtPlayer(index: %d) (перестать за ним бежать и аттаковать, потому что дистанция < %.1f и видно)", id, check_valid_attacker_distance[0]);
							}
							else if(IsAFireGun(CustomPedData[id][customPedWeapon][0])) PedUtils:ShootAtPlayer(id, INVALID_PLAYER_ID);
						}
						//else if(player_distance > check_valid_attacker_distance[0] && player_distance < check_valid_attacker_distance[1])
						else
						{
							if(CustomPedData[id][pedAttackingPlayer])
							{
								PedUtils:StopPlayerAttack(id, INVALID_PLAYER_ID);

								MovePedToPos(id, CustomPedData[id][customPedAttackerID]);

								SendClientMessageToAll(0xFF0000FF, "MovePedToPos(index: %d) (перестать его аттаковать и начать за ним бежать, потому что дистанция стала > %.1f или не видно)", id, check_valid_attacker_distance[0]);
							}
						}
					}
					else
					{
						PedUtils:StopPlayerAttack(id, INVALID_PLAYER_ID);

						CustomPedData[id][customPedAttackerID] = INVALID_PLAYER_ID;

						if(pedConnection != INVALID_PLAYER_ID) MovePedToPos(id, pedConnection);

						SendClientMessageToAll(0xFF0000FF, "PedMoveBack(index: %d) (перестать за ним бежать и перестать аттаковать, потому что дистанция > %.1f или не видно)", id, check_valid_attacker_distance[1]);
					}
				}
			}
			else
			{
				/*if(Iter_Contains(StoneManPeds, id) || Iter_Contains(SinisterZombies, id))
				{
					new Float:find_nearest_player = 30.0, Float:get_player_distance;

					foreach(PedStreamList[id], playerid)
					{
						get_player_distance = GetPlayerDistanceFromPoint(playerid, CustomPedData[id][customPedPosition][0], CustomPedData[id][customPedPosition][1], CustomPedData[id][customPedPosition][2]);

						if(get_player_distance < find_nearest_player) find_nearest_player = get_player_distance, CustomPedData[id][customPedAttackerID] = playerid;
					}
				}*/
				if(pedConnection != INVALID_PLAYER_ID && !CustomPedData[id][customPedVehicle][0] && GetPlayerState(pedConnection) == PLAYER_STATE_ONFOOT)
				{
					player_distance = GetPlayerDistanceFromPoint(pedConnection, CustomPedData[id][customPedPosition][0], CustomPedData[id][customPedPosition][1], CustomPedData[id][customPedPosition][2]),
					player_moving_distance = GetPlayerDistanceFromPoint(pedConnection, CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], CustomPedData[id][customPedMovingPosition][2]);

					if(player_distance > 30.0)
					{
						GetPlayerPos(pedConnection, CustomPedData[id][customPedTeleportPosition][0], CustomPedData[id][customPedTeleportPosition][1], CustomPedData[id][customPedTeleportPosition][2]);

						GetXYInFrontOfPlayer(pedConnection, CustomPedData[id][customPedTeleportPosition][0], CustomPedData[id][customPedTeleportPosition][1], -1.0);

						PedUtils:SetPedPos(id, INVALID_PLAYER_ID);

						SendClientMessageToAll(0xFF0000FF, "SetPedPos: телепортируем педа на координаты {%f, %f, %f}", CustomPedData[id][customPedTeleportPosition][0], CustomPedData[id][customPedTeleportPosition][1], CustomPedData[id][customPedTeleportPosition][2]);
					}
					else if(player_distance > 2.0 && player_moving_distance > 2.0)
					{
						GetPlayerPos(pedConnection, CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], CustomPedData[id][customPedMovingPosition][2]);

						GetXYInFrontOfPlayer(pedConnection, CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], -1.5);

						PedUtils:MovePedToPos(id, INVALID_PLAYER_ID);

						SendClientMessageToAll(0xFF0000FF, "MovePedToPos: отправляем педа на координаты {%f, %f, %f}", CustomPedData[id][customPedMovingPosition][0], CustomPedData[id][customPedMovingPosition][1], CustomPedData[id][customPedMovingPosition][2]);
					}
					CustomPedData[id][pedInterior] = GetPlayerInterior(pedConnection);
					CustomPedData[id][pedVirtualWorld] = GetPlayerVirtualWorld(pedConnection);
				}
			}
		}

		if(CustomPedData[id][customPedVehicleEntered])
			PedUtils:UpdateInCarPedSync(id, INVALID_PLAYER_ID);
		else
			PedUtils:UpdateOnFootPedSync(id, false, INVALID_PLAYER_ID);

		if(CustomPedData[id][customPedHealth][0] <= 0.0)
		{
			CustomPedData[id][customPedDeathStage] ++;
			if(CustomPedData[id][customPedDeathStage] == 10)
			{
				CustomPedData[id][customPedDeathStage] = 0;

				DestroyServerPed(id);

				Iter_SafeRemove(CustomPed, currentID, id);
			}
		}
	}
}