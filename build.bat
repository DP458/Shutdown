@echo off
msbuild /nologo .\Shutdown.sln /t:Rebuild /p:"Configuration=Release" /p:"Platform=x64"
