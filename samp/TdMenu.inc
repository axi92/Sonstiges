/*
Amirab's TextDraw Menu
Version 1.0
Made By Amirab
Skype : amirab_best
Yahoo ID : amirahmady1378
Please Leave My Credits
ShowTextDrawMenu(playerid, menuid, title[], lines, options)
//playerid   ===>   nothing for explaining xD all you know
//menuid    ===>   for set and use as Menuid , like defining for it . like a dialogid
//title        ===>   For setting title on top of your TD Menu
//lines       ===>   For setting how many your TD Menu lines are , it's actually for size of box and close message
//options   ===>   Define each number for list item , and if you set this on 0 close message is just



ShowTextDrawMenuItems(playerid, row, stats[], column1[], column2[])
//playerid   ===>   nothing for explaining xD all you know
//row        ===>   for putting string on that row
//stats      ===>   Setting stats for your TD Menu
//column1  ===>   for putting your string in column 1
//column2  ===>   for putting your string in column 2
//NOTE : this function should use under ShowTextDrawMenu !  



HideTextDrawMenu(playerid)
//playerid   ===>   nothing for explaining xD all you know

//This function is for hiding current showing TD Menu  
*/


#include <a_samp>


#define MAX_TEXTDRAW_ROWS 25

new Text:Menu_Title[MAX_PLAYERS];
new Text:Menu_Stats[MAX_PLAYERS];
new Text:Menu_StatsCont[MAX_PLAYERS];
new Text:Menu_End[MAX_PLAYERS];
new Text:Menu_Column1[MAX_PLAYERS][MAX_TEXTDRAW_ROWS];
new Text:Menu_Column2[MAX_PLAYERS][MAX_TEXTDRAW_ROWS];
new Menu_Options[MAX_PLAYERS];
new Menu_CurrentOptionSlot[MAX_PLAYERS];


new Menu_ItemSellerid[MAX_PLAYERS];





enum tdpInfo
{
pTextdraw,
}

new tdPlayerInfo[MAX_PLAYERS][tdpInfo];


public OnGameModeInit()
{

	for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
	{


	Menu_Title[playerid] = TextDrawCreate(116.000000, 120.000000, " ");
	TextDrawAlignment(Menu_Title[playerid], 2);
	TextDrawBackgroundColor(Menu_Title[playerid], 0x00000077);
	TextDrawFont(Menu_Title[playerid], 3);
	TextDrawLetterSize(Menu_Title[playerid], 0.509999, 1.400000);
	TextDrawColor(Menu_Title[playerid], -1);
	TextDrawSetOutline(Menu_Title[playerid], 0);
	TextDrawSetProportional(Menu_Title[playerid], 1);
	TextDrawSetShadow(Menu_Title[playerid], 1);
	TextDrawUseBox(Menu_Title[playerid], 1);
	TextDrawBoxColor(Menu_Title[playerid], 0x00000077);
	TextDrawTextSize(Menu_Title[playerid], 23.000000, 206.000000);

	Menu_Stats[playerid] = TextDrawCreate(115.000000, 162.000000, " ");
	TextDrawAlignment(Menu_Stats[playerid], 2);
	TextDrawBackgroundColor(Menu_Stats[playerid], 0x00000077);
	TextDrawFont(Menu_Stats[playerid], 1);
	TextDrawLetterSize(Menu_Stats[playerid], 0.240000, 1.200000);
	TextDrawColor(Menu_Stats[playerid], -1);
	TextDrawSetOutline(Menu_Stats[playerid], 0);
	TextDrawSetProportional(Menu_Stats[playerid], 1);
	TextDrawSetShadow(Menu_Stats[playerid], 1);

	Menu_StatsCont[playerid] = TextDrawCreate(115.000000, 162.000000, " ");
	TextDrawAlignment(Menu_StatsCont[playerid], 2);
	TextDrawBackgroundColor(Menu_StatsCont[playerid], 0x00000077);
	TextDrawFont(Menu_StatsCont[playerid], 1);
	TextDrawLetterSize(Menu_StatsCont[playerid], 0.240000, 1.200000);
	TextDrawColor(Menu_StatsCont[playerid], -1);
	TextDrawSetOutline(Menu_StatsCont[playerid], 0);
	TextDrawSetProportional(Menu_StatsCont[playerid], 1);
	TextDrawSetShadow(Menu_StatsCont[playerid], 1);

	Menu_End[playerid] = TextDrawCreate(115.000000, 202.000000, " ");
	TextDrawAlignment(Menu_End[playerid], 2);
	TextDrawBackgroundColor(Menu_End[playerid], 0x00000077);
	TextDrawFont(Menu_End[playerid], 1);
	TextDrawLetterSize(Menu_End[playerid], 0.240000, 1.200000);
	TextDrawColor(Menu_End[playerid], -1);
	TextDrawSetOutline(Menu_End[playerid], 0);
	TextDrawSetProportional(Menu_End[playerid], 1);
	TextDrawSetShadow(Menu_End[playerid], 1);



	new Float:ChatBoxY = 162.000000;

    for(new r = 0; r < MAX_TEXTDRAW_ROWS; r++)
	{

	Menu_Column1[playerid][r] = TextDrawCreate(22.000000, ChatBoxY, "~y~1 ~w~- ~g~Item 1");
	TextDrawBackgroundColor(Menu_Column1[playerid][r], 0x00000077);
	TextDrawFont(Menu_Column1[playerid][r], 1);
	TextDrawLetterSize(Menu_Column1[playerid][r], 0.240000, 1.200000);
	TextDrawColor(Menu_Column1[playerid][r], -1);
	TextDrawSetOutline(Menu_Column1[playerid][r], 0);
	TextDrawSetProportional(Menu_Column1[playerid][r], 1);
	TextDrawSetShadow(Menu_Column1[playerid][r], 1);

	Menu_Column2[playerid][r] = TextDrawCreate(162.000000, ChatBoxY, "~b~$100000000");
	TextDrawBackgroundColor(Menu_Column2[playerid][r], 0x00000077);
	TextDrawFont(Menu_Column2[playerid][r], 1);
	TextDrawLetterSize(Menu_Column2[playerid][r], 0.240000, 1.200000);
	TextDrawColor(Menu_Column2[playerid][r], -1);
	TextDrawSetOutline(Menu_Column2[playerid][r], 0);
	TextDrawSetProportional(Menu_Column2[playerid][r], 1);
	TextDrawSetShadow(Menu_Column2[playerid][r], 1);

	ChatBoxY += 10.000000;

	}
	
	}
	
	return CallLocalFunction("m_OnGameModeInit"  ,"");
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit m_OnGameModeInit
forward m_OnGameModeInit(playerid);





public OnPlayerText(playerid, text[])
{

	if(tdPlayerInfo[playerid][pTextdraw] > -1)
	{
	new option = strval(text);
	CallLocalFunction("OnPlayerEnterTextDrawMenu" , "iii" ,playerid,tdPlayerInfo[playerid][pTextdraw] , option);
	return 0;
	}

	return CallLocalFunction("m_OnPlayerText", "is", playerid, text);
}

#if defined _ALS_OnPlayerText
    #undef OnPlayerText
#else
    #define _ALS_OnPlayerText
#endif
#define OnPlayerText m_OnPlayerText
forward m_OnPlayerText(playerid, text[]);



public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_FIRE)
	{
	if(tdPlayerInfo[playerid][pTextdraw] > -1)
	{
	HideTextDrawMenu(playerid);
	return 0;
	}
	}

	return CallLocalFunction("m_OnPlayerKeyStateChange", "iii", playerid, newkeys , oldkeys);
}

#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange m_OnPlayerKeyStateChange
forward m_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);




stock ShowTextDrawMenu(playerid, menuid, title[], lines, options)
{
	new string[256];

	switch (lines)
	{
	case 1: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 2: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 3: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 4: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 5: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 6: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 7: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 8: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 9: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 10: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 11: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 12: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 13: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 14: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 15: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 16: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 17: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 18: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 19: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 20: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 21: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 22: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 23: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	case 24: format(string, sizeof(string), " ~n~%s~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ", title);
	}

	TextDrawSetString(Menu_Title[playerid], string);
	TextDrawShowForPlayer(playerid, Menu_Title[playerid]);


	if (options > 0)
	{
	format(string, sizeof(string), "(~y~T~w~) Or (~y~F6~w~) To Enter Number~n~Press ~w~(~y~LMB~w~) ~w~To Close");
	}else{
	format(string, sizeof(string), "Press ~w~(~y~LMB~w~) ~w~To Close");
	}

	switch (lines)
	{
	case 1: format(string, sizeof(string), "%s", string);
	case 2: format(string, sizeof(string), "%s", string);
	case 3: format(string, sizeof(string), " ~n~%s", string);
	case 4: format(string, sizeof(string), " ~n~ ~n~%s", string);
	case 5: format(string, sizeof(string), " ~n~ ~n~ ~n~%s", string);
	case 6: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~%s", string);
	case 7: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 8: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 9: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 10: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 11: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 12: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 13: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 14: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 15: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 16: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 17: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 18: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 19: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 20: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 21: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 22: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 23: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	case 24: format(string, sizeof(string), " ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~%s", string);
	}

	TextDrawSetString(Menu_End[playerid], string);
	TextDrawShowForPlayer(playerid, Menu_End[playerid]);

	Menu_Options[playerid] = options;
	tdPlayerInfo[playerid][pTextdraw] = menuid;

}
//==============================================================================
stock ShowTextDrawMenuItems(playerid, row, stats[], column1[], column2[])
{

	if (row == 0)
	{
	TextDrawSetString(Menu_Stats[playerid], stats);
	TextDrawSetString(Menu_StatsCont[playerid], " ");

	TextDrawShowForPlayer(playerid, Menu_Stats[playerid]);
	TextDrawShowForPlayer(playerid, Menu_StatsCont[playerid]);
	}else{

	TextDrawSetString(Menu_Column1[playerid][row], column1);
	TextDrawSetString(Menu_Column2[playerid][row], column2);
	TextDrawShowForPlayer(playerid, Menu_Column1[playerid][row]);
	TextDrawShowForPlayer(playerid, Menu_Column2[playerid][row]);

	}
	
}
//==============================================================================
stock HideTextDrawMenu(playerid)
{

	TextDrawHideForPlayer(playerid, Menu_Title[playerid]);
	TextDrawHideForPlayer(playerid, Menu_Stats[playerid]);
	TextDrawHideForPlayer(playerid, Menu_StatsCont[playerid]);
	TextDrawHideForPlayer(playerid, Menu_End[playerid]);

    for(new r = 0; r < MAX_TEXTDRAW_ROWS; r++)
	{
	TextDrawHideForPlayer(playerid, Menu_Column1[playerid][r]);
	TextDrawHideForPlayer(playerid, Menu_Column2[playerid][r]);
	}



	Menu_CurrentOptionSlot[playerid] = 0;
	tdPlayerInfo[playerid][pTextdraw] = -1;
	Menu_ItemSellerid[playerid] = -1;

}
//==============================================================================


stock isNumeric3(const string[])
{
  new length=strlen(string);
  if (length==0) return false;
  for (new i = 0; i < length; i++)
    {
      if (
            (string[i] > '9' || string[i] < '0' && string[i]!='-' && string[i]!='+') // Not a number,'+' or '-'
             || (string[i]=='-' && i!=0)                                             // A '-' but not at first.
             || (string[i]=='+' && i!=0)                                             // A '+' but not at first.
         ) return false;
    }
  if (length==1 && (string[0]=='-' || string[0]=='+')) return false;
  return true;
}

forward OnPlayerEnterTextDrawMenu(playerid, menuid , listitem);