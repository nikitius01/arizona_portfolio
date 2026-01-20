#if defined _custom_dialog_inc
    #endinput
#endif
#define _custom_dialog_inc

#define CustomDialog: customdi_


stock CustomDialog:ShowPlayerDialog(playerid, dialogid, style, const caption[], const info[], const button1[], const button2[])
{
    if(!IsLauncherConnected(playerid) && !IsTrilogyConnected(playerid))
        return 0;

    if(IsModalTypeOpened(playerid, MODAL_TYPE_DIALOG)) HidePlayerModal(playerid, MODAL_TYPE_DIALOG, true);

    format(mainString, 4096, 
        "\
            {\
                \"id\":%d,\
                \"type\":%d,\
                \"header\":\"%s\",\
                \"body\":\"%s\",\
                \"primaryButton\":\"%s\",\
                \"secondaryButton\":\"%s\",\
                \"mode\":0\
            }\
        ",
        dialogid,
        style,
        caption,
        info,
        button1,
        button2
    );

    ShowPlayerModal(playerid, MODAL_TYPE_DIALOG, true, mainString);

    if(style == DIALOG_STYLE_INPUT || style == DIALOG_STYLE_PASSWORD) TogglePlayerControllable(playerid, false);

    return 1;
}