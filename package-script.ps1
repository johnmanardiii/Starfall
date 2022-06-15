Copy-Item -Path ./resources/ -Destination ./build/Release -recurse -force
New-Item -ItemType File -Path ./build/Release/executable/Starfall.exe -Force
Move-Item ./build/Release/Starfall.exe -Destination  ./build/Release/executable/Starfall.exe -Force
Compress-Archive ./build/Release -DestinationPath ./build/Release/Release
