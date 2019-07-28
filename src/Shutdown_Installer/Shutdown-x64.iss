#define Architecture "x64"

#include "Shutdown.iss"

#define MyAppId "{{A35D5E02-82FD-4B91-971D-5CEC5B9428A4}"

[Setup]
AppId="{#MyAppId}"
SetupMutex="{#MyAppId},Global\{#MyAppId}"
ArchitecturesAllowed="x64"
ArchitecturesInstallIn64BitMode="x64"

[Files]
Source: "{#MyProjectFolder}\{#Architecture}\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion