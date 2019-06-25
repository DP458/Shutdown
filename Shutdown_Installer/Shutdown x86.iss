#define Architecture "x86"

#include "Shutdown.iss"

#define MyAppId "{{B16C89EB-C33C-4C81-B846-D850A0D9EB14}"

[Setup]
AppId="{#MyAppId}"
SetupMutex="{#MyAppId},Global\{#MyAppId}"

[Files]
Source: "{#MyProjectFolder}\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion