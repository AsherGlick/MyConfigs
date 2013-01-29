#!/bin/sh
echo "Creating Launcher"
echo "[Desktop Entry]" >> /usr/share/applications/sublime.desktop
echo "Version=1.0" >> /usr/share/applications/sublime.desktop
echo "Name=Sublime Text 2" >> /usr/share/applications/sublime.desktop
echo "GenericName=Text Editor" >> /usr/share/applications/sublime.desktop
echo "Exec=sublime" >> /usr/share/applications/sublime.desktop
echo "Terminal=false" >> /usr/share/applications/sublime.desktop
echo "Icon=/opt/Sublime Text 2/Icon/48x48/sublime_text.png" >> /usr/share/applications/sublime.desktop
echo "Type=Application" >> /usr/share/applications/sublime.desktop
echo "Categories=TextEditor;IDE;Development" >> /usr/share/applications/sublime.desktop
echo "X-Ayatana-Desktop-Shortcuts=NewWindow" >> /usr/share/applications/sublime.desktop
echo "[NewWindow Shortcut Group]" >> /usr/share/applications/sublime.desktop
echo "Name=New Window" >> /usr/share/applications/sublime.desktop
echo "Exec=sublime -n" >> /usr/share/applications/sublime.desktop
echo "TargetEnvironment=Unity" >> /usr/share/applications/sublime.desktop
