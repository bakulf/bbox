/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_CONST_H_
#define _BB_CONST_H_

#define BB_ICON_IMAGE	":/images/icon.png"
#define BB_BLINK_IMAGE	":/images/blink.png"
#define BB_LOGO_IMAGE	":images/logo.png"
#define BB_PREFS_IMAGE	":images/preferences.png"

#define BB_STATE_WAITING_IMAGE	":images/waiting.png"
#define BB_STATE_RUNNING_IMAGE	":images/running.gif"
#define BB_STATE_ERROR_IMAGE	":images/error.png"
#define BB_STATE_COMPLETE_IMAGE	":images/completed.png"

#define BB_SVN_ADD_MAX 15
#define BB_SVN_DIR     ".svn"

#define BB_SVN_CONFIG_DIR     "bbox"
#define BB_SVN_CONFIG_FILE    "config"
#define BB_SVN_CONFIG_CONTENT ":config/config"

#define BB_COMMIT_TIMEOUT   10000 /* 10 seconds */
#define BB_OBSERVER_TIMEOUT 5000  /* 5 seconds */
#define BB_BLINK_TIMEOUT    1000  /* 1 second */

#define BB_LINUX_STARTUP_PATH    ".config/autostart"
#define BB_LINUX_STARTUP_FILE    "bbox.desktop"
#define BB_LINUX_STARTUP_CONTENT QString( \
"[Desktop Entry]\n" \
"Version=1.0\n" \
"Encoding=UTF-8\n" \
"Name=BBox\n" \
"Type=Application\n" \
"Exec=bbox.n3\n" \
"Terminal=false\n" \
"Icon=bbox\n" \
"Comment=...something like dropbox based on subversion.\n" \
"Categories=Utility;\n" \
"Categories=Network;\n" \
"Hidden=false\n" \
)

#define BB_MAC_STARTUP_PATH    "Library/LaunchAgents"
#define BB_MAC_STARTUP_FILE    "com.nois3lab.bbox.plist"
#define BB_MAC_STARTUP_CONTENT QString( \
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
  "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n" \
  "<plist version=\"1.0\">\n" \
  "  <dict>\n" \
  "    <key>Label</key>\n" \
  "    <string>com.nois3lab.bbox</string>\n" \
  "    <key>OnDemand</key>\n" \
  "    <false/>\n" \
  "    <key>ProgramArguments</key>\n" \
  "    <array>\n" \
  "      <string>%1</string>\n" \
  "    </array>\n" \
  "  </dict>\n" \
  "</plist>" \
)

#define BB_WIN_STARTUP_REGEDIT "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define BB_WIN_STARTUP_KEY     "BBox"

#endif
