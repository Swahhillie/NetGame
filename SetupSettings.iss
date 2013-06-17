; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Network Game"
#define MyAppVersion "1.5"
#define MyAppPublisher "David Hoogenbosch"
#define MyAppURL "www.davidhoogenbosch.com"
#define MyAppExeName "NetGame.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{21489248-4C4A-415F-8B8B-4C96AD9DD52A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\Network Game DavidH
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputBaseFilename=NetworkGameSetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\Users\David\Documents\GitHub\NetGame\NetGame.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\assets\*"; DestDir: "{app}/assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\Users\David\Documents\GitHub\NetGame\config.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\libsndfile-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\lvl2.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\netGameDb"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\openal32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Users\David\Documents\GitHub\NetGame\rectangleLevel.json"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

