/*
	Friend System
	© Jingles - 2015
	
	Make sure to import the friendsdb.sql into your SA:MP database.
	
	
	DB Info:
	db = `friendsdb`
	UserID = `UserDBID`
	FriendID = `FriendDBID`


#include <a_samp>
#include <a_mysql>
#include <foreach>
#include <ZCMD>

#define MYSQL_IP                        "127.0.0.1"
#define MYSQL_USERNAME                  "root"
#define MYSQL_DATABASE                  "samp"
#define MYSQL_PASS                      "INSERT_PASSWORD_HERE"

*/
#define MAX_FRIENDS 100

#define COLOR_WHITE 0xFFFFFFAA

#define	THREAD_ACCOUNT_LOADDATA            	(1)
#define THREAD_SAVE_ACCOUNT_1         		(2)
#define THREAD_NO_RESULT			  		(3)

#define ADD_FRIEND_THREAD					(50)
#define FINAL_FRIEND_THREAD					(51)
#define REQUEST_FRIEND_THREAD				(52)

enum {
	DIALOG_FRIENDLIST
}

new mysql_iConnectionHandle[1];
	
	
enum pData {
    pDBID,
	pUsername[MAX_PLAYER_NAME]
}
new PlayerInfo[MAX_PLAYERS][pData];

new szFriendList[MAX_FRIENDS * MAX_PLAYER_NAME + 30];

/*
public OnFilterScriptInit()
{
	mysql_iConnectionHandle[0] = mysql_connect(MYSQL_IP,  MYSQL_USERNAME, MYSQL_DATABASE, MYSQL_PASS);
	
	foreach(Player, playerid)
	{
		LoadData(playerid);
	}	
	return 1;
}

public OnFilterScriptExit()
{
	mysql_close(mysql_iConnectionHandle[0]);
	return 1;
}
*/


public OnPlayerConnect(playerid)
{
	LoadData(playerid);
	return 1;
}

forward LoadData(playerid);
public LoadData(playerid)
{
	new szQuery[128],
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	mysql_real_escape_string(name, PlayerInfo[playerid][pUsername], mysql_iConnectionHandle[0], MAX_PLAYER_NAME);
	format(szQuery, sizeof szQuery, "SELECT * FROM `accountdb` WHERE `Username` = '%s'", PlayerInfo[playerid][pUsername]);
    mysql_function_query(mysql_iConnectionHandle[0], szQuery, true, "OnQueryFinish", "ii", THREAD_ACCOUNT_LOADDATA, playerid);
	return 1;
}


CMD:friends(playerid, params[])
{
	new query[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "SELECT * FROM `friendsdb` WHERE `UserDBID` = %i", PlayerInfo[playerid][pDBID]);
	mysql_function_query(mysql_iConnectionHandle[0], query, true, "OnPlayerRequestFriends", "ii", REQUEST_FRIEND_THREAD, playerid);		
	return 1;
}



CMD:addfriend(playerid, params[])
{
	new giveplayerid,
		query[512];

	if(sscanf(params, "u", giveplayerid))
		return SendClientMessage(playerid, COLOR_WHITE, "/addfriend [player]");
	
	if(IsPlayerConnected(giveplayerid))
	{
		mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "SELECT * FROM `accountdb` WHERE `Username` = '%s'", GetPlayerNameExt(giveplayerid));
		mysql_function_query(mysql_iConnectionHandle[0], query, true, "OnPlayerRequestFriends", "ii", ADD_FRIEND_THREAD, playerid);		
	}
	else
	{
		mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "SELECT * FROM `accountdb` WHERE `Username` = '%s'", params);
		mysql_function_query(mysql_iConnectionHandle[0], query, true, "OnPlayerRequestFriends", "ii", ADD_FRIEND_THREAD, playerid);		
	}
	return 1;
}

forward OnPlayerRequestFriends(thread, playerid, Friend_ID);
public OnPlayerRequestFriends(thread, playerid, Friend_ID)
{
	switch(thread)
	{
		case THREAD_NO_RESULT:
		{
			return 1;
		}
		case ADD_FRIEND_THREAD:
		{
			new rows,
				fields;

			cache_get_data(rows, fields, mysql_iConnectionHandle[0]);

			if(rows) 
			{
				new	
					query[128],
					szResult[64],
					szFriendName[64],
					FriendDBID;

				for(new row; row < rows; row++)	{
					cache_get_field_content(row, "Username", szResult, mysql_iConnectionHandle[0], sizeof(szResult)); szFriendName = szResult;
					cache_get_field_content(row, "id", szResult, mysql_iConnectionHandle[0], sizeof(szResult)); FriendDBID = strval(szResult);
					
					/* DEBUG
					format(szResult, sizeof(szResult), "Found in DB: %s with ID: %i", szFriendName, FriendDBID);
					SendClientMessage(playerid, COLOR_WHITE, szResult);
					*/
					
					SetPVarInt(playerid, "New_Friend_DBID", FriendDBID);
					SetPVarString(playerid, "New_Friend_Name", szFriendName);
					mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "SELECT * FROM `friendsdb` WHERE `UserDBID` = %i AND `FriendDBID` = %i", PlayerInfo[playerid][pDBID], FriendDBID);
					mysql_function_query(mysql_iConnectionHandle[0], query, true, "OnPlayerRequestFriends", "ii", FINAL_FRIEND_THREAD, playerid);
					return 1;
				}
			}
			else {
				SendClientMessage(playerid, COLOR_WHITE, "That name does not exist.");
			}
			return 1;
		}
		case FINAL_FRIEND_THREAD:
		{
			new rows,
				fields;
				
			cache_get_data(rows, fields, mysql_iConnectionHandle[0]);

			if(!rows) {
				new query[128],
					FriendName[MAX_PLAYER_NAME+1];
				mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "INSERT INTO `friendsdb` (`UserDBID`, `FriendDBID`) VALUES (%i, %i)", PlayerInfo[playerid][pDBID], GetPVarInt(playerid, "New_Friend_DBID"));
				mysql_function_query(mysql_iConnectionHandle[0], query, false, "OnPlayerRequestFriends", "i", THREAD_NO_RESULT);
				
				GetPVarString(playerid, "New_Friend_Name", FriendName, sizeof FriendName);
				format(query, sizeof query, "Successfully added %s to your friendlist.", FriendName);
				SendClientMessage(playerid, COLOR_WHITE, query);
				
				DeletePVar(playerid, "New_Friend_Name");
				DeletePVar(playerid, "New_Friend_DBID");
				
				return 1;
			}
			else {
				SendClientMessage(playerid, COLOR_WHITE, "You already have a friend with that name.");
			}
			return 1;
		}
		case REQUEST_FRIEND_THREAD:
		{
			new rows,
				fields;

			cache_get_data(rows, fields, mysql_iConnectionHandle[0]);

			if(rows) {
				new	szResult[64];

				for(new row; row < rows; row++)	{
					cache_get_field_content(row, "FriendDBID", szResult, mysql_iConnectionHandle[0], sizeof(szResult));
					/* print("Friend IDs:");		
					print(szResult);		
					*/
					
					new query[128];
					mysql_format(mysql_iConnectionHandle[0], query, sizeof(query), "SELECT * FROM `accountdb` WHERE `id` = %i", strval(szResult));
					mysql_function_query(mysql_iConnectionHandle[0], query, true, "OnPlayerRequestFriendNames", "ii", playerid, strval(szResult));	

					/*if(strcmp(szResult, GetPlayerNameExt(extraid), true) != 0) continue;
					cache_get_field_content(row,  "ID", szResult, mysql_iConnectionHandle[0], sizeof szResult); playerData[extraid][pDBID] = strval(szResult);
					cache_get_field_content(row,  "ONotice", szResult, mysql_iConnectionHandle[0], sizeof szResult); SetPVarString(extraid, "PlayerONotice", szResult);
					*/
				}
			}
			return 1;
		}
	}
	return 1;
}

forward OnPlayerRequestFriendNames(playerid, Friend_DBID, thread);
public OnPlayerRequestFriendNames(playerid, Friend_DBID, thread)
{
	switch(thread)
	{
		case 0:
		{
			new rows,
			fields;

			cache_get_data(rows, fields, mysql_iConnectionHandle[0]);

			if(rows) {
				new	szResult[64];
				
				new 
					szPlayerName[MAX_PLAYER_NAME],
					iPhoneNumber,
					szTitle[64];
					
				SetPVarInt(playerid, "Friend_DBID", Friend_DBID);

				for(new row; row < rows; row++)	
				{
					cache_get_field_content(row, "Username", szPlayerName, mysql_iConnectionHandle[0], sizeof(szResult));
					cache_get_field_content(row, "PhoneNr", szResult, mysql_iConnectionHandle[0], sizeof(szResult)); iPhoneNumber = strval(szResult);

					foreach(Player, i) {
						if(PlayerInfo[i][pDBID] == Friend_DBID)	{
							format(szFriendList, sizeof(szFriendList), "%s\n* {6CBBE3}Name: {FFFFFF}%s | {8BA870}(online)", szFriendList, szPlayerName, iPhoneNumber);
						}
						else {
							format(szFriendList, sizeof(szFriendList), "%s\n* {6CBBE3}Name: {FFFFFF}%s | {FF0000}(offline)", szFriendList, szPlayerName, iPhoneNumber);
						}
					}
				}
				format(szTitle, sizeof szTitle, "%s's Friendlist", getPlayerName(playerid));
				ShowPlayerDialog(playerid, DIALOG_FRIENDLIST, DIALOG_STYLE_LIST, szTitle, szFriendList, "Select", "Cancel");
			}
		}
		case 1:
		{
		
		}
	}
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_FRIENDLIST) 
	{
		strdel(szFriendList, 0, strlen(szFriendList));
	}
	return 0;
}
	
	
forward OnQueryFinish(resultid, extraid);
public OnQueryFinish(resultid, extraid) {
	switch(resultid) {
		case THREAD_ACCOUNT_LOADDATA: {
			if(IsPlayerConnected(extraid)) {
			

				new rows,
				    fields;

			    cache_get_data(rows, fields, mysql_iConnectionHandle[0]);

			    if(rows) {
	   				new	szResult[64];

					for(new row; row < rows; row++)	{
						cache_get_field_content(row, "id", szResult, mysql_iConnectionHandle[0], MAX_PLAYER_NAME); PlayerInfo[extraid][pDBID] = strval(szResult);

						/*if(strcmp(szResult, GetPlayerNameExt(extraid), true) != 0) continue;
						cache_get_field_content(row,  "ID", szResult, mysql_iConnectionHandle[0], sizeof szResult); playerData[extraid][pDBID] = strval(szResult);
						cache_get_field_content(row,  "ONotice", szResult, mysql_iConnectionHandle[0], sizeof szResult); SetPVarString(extraid, "PlayerONotice", szResult);
						*/
						break;
					}
				}
			}
			return 1;
		}
		case THREAD_NO_RESULT: { return 1; }
		/*case THREAD_SAVE_ACCOUNT_1: {

		    new
				szQuery[2048];
		    format(szQuery, sizeof(szQuery), "UPDATE `friendsdb` SET `FriendDBID` = %d WHERE `UserDBID` = %i", playerData[extraid][pDBID], playerData[);
			return mysql_function_query(mysql_iConnectionHandle[0], szQuery, false, "OnQueryFinish", "iii", THREAD_SAVE_ACCOUNT_2, extraid, g_iPlayerHandle[extraid]);
		}*/
	}
	return 1;
}

stock GetPlayerNameExt(playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock getPlayerName(playerid, underscore = 0) {
	new
		szName[MAX_PLAYER_NAME],
		iPos;

	GetPlayerName(playerid, szName, MAX_PLAYER_NAME);

	if(!underscore) while((iPos = strfind(szName, "_", false, iPos)) != -1) szName[iPos] = ' ';

	return szName;
}

