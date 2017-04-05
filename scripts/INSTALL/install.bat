
ECHO This will install vcpkg on your Windows disk
SET vcpkgdir=%SYSTEMDRIVE%\vcpkg
git clone https://github.com/Microsoft/vcpkg.git %vcpkgdir%


RUN %vcpkgdir%\bootstrap-vcpkg.bat
start %vcpkgdir%\vcpkg.exe install sfml:x64-windows rapidjson:x64-windows

