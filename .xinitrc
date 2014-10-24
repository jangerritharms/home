#!/bin/sh
#
# ~/.xinitrc
#
# Executed by startx (run your window manager from here)

if [ -d /etc/X11/xinit/xinitrc.d ]; then
  for f in /etc/X11/xinit/xinitrc.d/*; do
    [ -x "$f" ] && . "$f"
  done
  unset f
fi

xmodmap ~/.xmodmap
xrdb -merge ~/.Xresources & 
feh --bg-scale ~/wallpaper.jpg &
pulse-audio --start &
dropboxd & 
/usr/local/bin/dwmstatus &

# exec gnome-session
# exec startkde
# exec startxfce4
# ...or the Window Manager of your choice
exec dwm
