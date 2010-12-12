[Setup]
AppName=BBox
AppVersion=0.1
DefaultDirName={pf}\BBox
DefaultGroupName=BBox
UninstallDisplayIcon={app}\BBox.exe

[Files]
Source: "../win32/package/BBox.exe"; DestDir: "{app}"
Source: "../win32/package/*.dll"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\BBox"; Filename: "{app}\BBox.exe"