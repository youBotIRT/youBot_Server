#!/bin/sh

gnome-terminal --title="JoyPad"   --command="../youBot_JoyPad/bin/youBot_JoyPad"&
gnome-terminal --title="UDP 6500" --command="../youBot_UDP/bin/youBot_UDP 6500"&
gnome-terminal --title="UDP 6501" --command="../youBot_UDP/bin/youBot_UDP 6501"&
gnome-terminal --title="UDP 6502" --command="../youBot_UDP/bin/youBot_UDP 6502"&
gnome-terminal --title="UDP 6503" --command="../youBot_UDP/bin/youBot_UDP 6503"&
gnome-terminal --title="Info" --command="../youBot_Info/bin/youBot_Info"&
gnome-terminal --title="Server" --command="../youBot_Server/bin/youBot_Server"
