enum sannewsevent {
	iseventopen,
	Float:newseventport_x,
	Float:newseventport_y,
	Float:newseventport_z
}

CMD:newsevent(playerid, params[])
{
	if(GetPlayerInterior(playerid) != 0) return SendClientMessage(playerid, COLOR_RED, "Du bist in einen Interior bitte verlasse das Interior!");
	new sannewsmsg[200];
	if(GetPlayerFraktionID(playerid) == 9 && PlayerInfo[playerid][pRank] > 9) 
	{
		if(sannewsevent[iseventopen] == 0)
		{
			sannewsevent[iseventopen] = 1;
			GetPlayerPos(playerid, sannewsevent[newseventport_x], sannewsevent[newseventport_y], sannewsevent[newseventport_z]);
			SetPlayerCheckpoint(playerid, sannewsevent[newseventport_x], sannewsevent[newseventport_y], sannewsevent[newseventport_z], 3.0);
			format(sannewsmsg, sizeof(sannewsmsg, "NR %s hat ein Event gestartet, benutze /sevent um auf der Map einen Checkpoint zu bekommen.", PlayerInfo[playerid][pName]);
			return SendClientMessageToAll(COLOR_ORANGE, sannewsmsg);
		else 
		{
			sannewsevent[iseventopen] = 0;
			sannewsevent[newseventport_x] = 0;
			sannewsevent[newseventport_y] = 0;
			sannewsevent[newseventport_z] = 0;
			format(sannewsmsg, sizeof(sannewsmsg, "NR %s hat das Event geschlossen.", PlayerInfo[playerid][pName]);
			return SendClientMessageToAll(COLOR_ORANGE, sannewsmsg);
		}
	}
	return 1;
}
CMD:sevent(playerid, params[])
{
	if(sannewsevent[iseventopen] == 1)
	{
		SetPlayerCheckpoint(playerid, sannewsevent[newseventport_x], sannewsevent[newseventport_y], sannewsevent[newseventport_z], 3.0);
		SendClientMessage(playerid, COLOR_YELLOW, "Es wurde ein Checkpoint beim Event gesetzt.");
	}
	return 1;
} 