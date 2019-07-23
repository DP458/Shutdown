@echo off
msbuild /nologo ..\src\Shutdown.sln /t:Rebuild /p:"Configuration=Release" /p:"Platform=x64"
