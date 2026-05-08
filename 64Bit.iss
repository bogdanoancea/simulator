; -- 64Bit.iss --
; Inno Setup installer for Mobile Network Event Simulator
; 64-bit Windows build

#define MyAppName "Net Event Simulator"
#define MyAppVersion "1.3.0"
#define MyAppPublisher "Mobile Network Event Simulator Project"
#define MyAppExeName "simulator.exe"

; Main project paths
#define ProjectDir "D:\c-projects\simulator"
#define ReleaseDir "D:\c-projects\simulator\Release"
#define DataDir "D:\c-projects\simulator\data"
#define OutputDir "D:\c-projects\simulator\installer"

[Setup]
AppId={{B8C8E4B7-0B52-49F2-8E76-7C7A13A4D33A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}

DefaultDirName={autopf}\Simulator
DefaultGroupName=Simulator
DisableProgramGroupPage=yes

OutputDir={#OutputDir}
OutputBaseFilename=NetworkEventSimulator-{#MyAppVersion}-x64-Setup

Compression=lzma2
SolidCompression=yes
WizardStyle=modern

UninstallDisplayIcon={app}\{#MyAppExeName}

ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

PrivilegesRequired=admin
UsePreviousAppDir=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Dirs]
Name: "{app}\Data"

[Files]
; Main executable
Source: "{#ReleaseDir}\{#MyAppExeName}"; DestDir: "{app}"; DestName: "{#MyAppExeName}"; Flags: ignoreversion

; Runtime DLLs copied next to simulator.exe
; This covers MSYS2 / UCRT64 / Rtools dependencies such as:
; libstdc++-6.dll, libgcc_s_seh-1.dll, libwinpthread-1.dll,
; libgeos.dll, libgeos_c.dll, etc.
Source: "{#ReleaseDir}\*.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

; Data directory
Source: "{#DataDir}\*"; DestDir: "{app}\Data"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist

; Optional documentation
Source: "{#ProjectDir}\README*"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "{#ProjectDir}\LICENSE*"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Icons]
; Start Menu shortcut: opens a command prompt, runs simulator.exe, and keeps the window open
Name: "{group}\Simulator"; Filename: "{cmd}"; Parameters: "/K ""{app}\{#MyAppExeName}"""; WorkingDir: "{app}"

; Optional desktop shortcut
Name: "{autodesktop}\Simulator"; Filename: "{cmd}"; Parameters: "/K ""{app}\{#MyAppExeName}"""; WorkingDir: "{app}"; Tasks: desktopicon

; Uninstaller shortcut
Name: "{group}\Uninstall Simulator"; Filename: "{uninstallexe}"

[Run]
; Optional: launch after installation
Filename: "{cmd}"; Parameters: "/K ""{app}\{#MyAppExeName}"""; WorkingDir: "{app}"; Description: "Launch Simulator"; Flags: postinstall skipifsilent unchecked
