#define MyAppName "Shutdown"
#define MyAppPublisher "DP458"
#define MyAppVersion "7.6.2"
#define MyAppExeName "Shutdown.exe"
#define MyProjectFolder "..\"
#define QuickLaunchFolder "{userappdata}\Microsoft\Internet Explorer\Quick Launch"

[Languages]
Name: "English"; MessagesFile: "Languages\CustomDefault.isl"
Name: "Russian"; MessagesFile: "Languages\CustomRussian.isl"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppComments={cm:Shutdown}
AppPublisher={#MyAppPublisher}
VersionInfoVersion={#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
UninstallDisplayName={cm:Shutdown}
OutputDir="bin"
Compression=none
SolidCompression=no
MinVersion="0,6.1.7600"
UninstallDisplayIcon="{app}\{#MyAppExeName}"
ShowLanguageDialog=Yes
WizardSmallImageFile="Images\WizModernSmallImage-Shutdown.bmp"
WizardImageStretch=no
WizardImageAlphaFormat="premultiplied"
DisableWelcomePage=false
AllowCancelDuringInstall=no    
DisableProgramGroupPage=yes
DisableReadyPage=yes
DisableDirPage=yes
ShowTasksTreeLines=yes
ASLRCompatible=yes
DEPCompatible=yes
PrivilegesRequired=admin

[Tasks]
Name: "ShutdownQuickLaunchBarIcon"; Description: "{cm:ShutdownShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "checkablealone";
Name: "RebootQuickLaunchBarIcon"; Description: "{cm:RebootShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "checkablealone"; 
Name: "LogoffQuickLaunchBarIcon"; Description: "{cm:LogoffShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "unchecked";
Name: "LockQuickLaunchBarIcon"; Description: "{cm:LockShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "unchecked";
Name: "SuspendQuickLaunchBarIcon"; Description: "{cm:SuspendShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "unchecked";
Name: "HibernationQuickLaunchBarIcon"; Description: "{cm:HibernationShortcut}"; GroupDescription: "{cm:QuickLaunchBarShortcuts}"; Flags: "unchecked";

Name: "ShutdownWindowsDialogDesktopIcon"; Description: "{cm:ShutdownWindowsDialogShortcut}"; GroupDescription: "{cm:DialogDesktopShortcuts}"; Flags: "unchecked";
Name: "ShutdownDialogDesktopIcon"; Description: "{cm:ShutdownDialogShortcut}"; GroupDescription: "{cm:DialogDesktopShortcuts}"; Flags: "unchecked";

Name: "ShutdownDesktopIcon"; Description: "{cm:ShutdownShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "checkablealone";
Name: "RebootDesktopIcon"; Description: "{cm:RebootShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "checkablealone"; 
Name: "LogoffDesktopIcon"; Description: "{cm:LogoffShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "unchecked";
Name: "LockDesktopIcon"; Description: "{cm:LockShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "unchecked";
Name: "SuspendDesktopIcon"; Description: "{cm:SuspendShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "unchecked";
Name: "HibernationDesktopIcon"; Description: "{cm:HibernationShortcut}"; GroupDescription: "{cm:CertainDesktopShortcuts}"; Flags: "unchecked";

[Icons]
Name: "{commondesktop}\{cm:Shutdown}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:0; Check: AddDesktopShutdownDialogShortcut;

Name: "{#QuickLaunchFolder}\{cm:Shutdown}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "S"; Tasks: "ShutdownQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:0;
Name: "{#QuickLaunchFolder}\{cm:Reboot}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "R"; Tasks: "RebootQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:1;
Name: "{#QuickLaunchFolder}\{cm:Logoff}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "L"; Tasks: "LogoffQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:2;
Name: "{#QuickLaunchFolder}\{cm:Lock}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "l"; Tasks: "LockQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:3;
Name: "{#QuickLaunchFolder}\{cm:Suspend}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "s"; Tasks: "SuspendQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:5;
Name: "{#QuickLaunchFolder}\{cm:Hibernation}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "H"; Tasks: "HibernationQuickLaunchBarIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:6;

Name: "{commondesktop}\{cm:ShutdownWindowsDialog}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "D"; Tasks: "ShutdownWindowsDialogDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:4;
Name: "{commondesktop}\{cm:ShutdownDialog}"; Filename: "{app}\{#MyAppExeName}"; Tasks: "ShutdownDialogDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:0;

Name: "{commondesktop}\{cm:Shutdown}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "S"; Tasks: "ShutdownDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:0;
Name: "{commondesktop}\{cm:Reboot}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "R"; Tasks: "RebootDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:1;
Name: "{commondesktop}\{cm:Logoff}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "L"; Tasks: "LogoffDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:2;
Name: "{commondesktop}\{cm:Lock}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "l"; Tasks: "LockDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:3;
Name: "{commondesktop}\{cm:Suspend}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "s"; Tasks: "SuspendDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:5;
Name: "{commondesktop}\{cm:Hibernation}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "H"; Tasks: "HibernationDesktopIcon"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex:6;

[Code]
function AddDesktopShutdownDialogShortcut(): Boolean;
begin

Result := not
(
  IsTaskSelected('ShutdownWindowsDialogDesktopIcon')
  or IsTaskSelected('ShutdownDialogDesktopIcon')
  or IsTaskSelected('ShutdownDesktopIcon')
  or IsTaskSelected('RebootDesktopIcon')
  or IsTaskSelected('LogoffDesktopIcon')
  or IsTaskSelected('LockDesktopIcon')
  or IsTaskSelected('SuspendDesktopIcon')
  or IsTaskSelected('HibernationDesktopIcon')
);

end;

#ifdef Architecture

[Setup]
OutputBaseFilename="Shutdown_Setup_{#Architecture}"
SetupIconFile="Icons\Shutdown_Setup_{#Architecture}.ico"
WizardImageFile="Images\WizModernImage-Shutdown-{#Architecture}.bmp"

#else

# error  Директива Architecture не определена

#endif
