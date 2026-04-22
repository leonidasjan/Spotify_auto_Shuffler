## Spotify auto-shuffler

Light-Weight, fast shuffler for your playlists

it uses:
std::random_device
std::mt19937

for true randomness

It uses these scopes:
playlist-read-private
playlist-modify-private
playlist-modify-public
(you can read about them when authenticating)

 Compatibile with spotify API. (4/22/2026)

## Build

mkdir build
cd build
cmake ..
cmake --build .
cpp23+

## Run

./main.exe (if you have MSYS2 installed)

OR

download zip file , extract it and run the .exe

Spotify made a change and now it requies you to have spotify premium account to use this tool.
Read more: https://developer.spotify.com/blog/2026-02-06-update-on-developer-access-and-platform-security

## Autostart tutorial (Windows)

Create a shortcut to main.exe file

Right click on shortcut -> Properties and serach for [Target] box

At the end of the path, add: --autostart (you can also add option to run this program minimized so it wont pop up)

Click apply

Now, WIN+R -> shell:startup and add the shortcut here

Done!
