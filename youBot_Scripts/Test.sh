#!/bin/sh



gnome-terminal --title="Server"  -e  "../youBot_Server/bin/youBot_Server" --geometry 48x15+50+50

gnome-terminal --title="JoyPad"  -e  "../youBot_JoyPad/bin/youBot_JoyPad" --geometry 48x15+490+50

gnome-terminal --title="Monitor" -e  "../youBot_Info/bin/youBot_Info" --geometry 48x15+940+50

gnome-terminal --title="UDP 6502" -e "../youBot_UDP/bin/youBot_UDP 6502" --geometry 48x15+940+50

gnome-terminal --title="Monitor" -e  "../youBot_Monitor/bin/youBot_Monitor" --geometry 140x19+50+350


