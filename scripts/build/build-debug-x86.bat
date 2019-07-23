@echo off
msbuild /nologo ..\src\Shutdown.sln /t:Rebuild /p:"Configuration=Debug" /p:"Platform=x86"
