# Engrave - False engrave images with GEGL
Create an engrave effect. Filter is located in Filters>Artistic>Engrave

![image](https://github.com/user-attachments/assets/dbc11709-cdcd-4d43-a7a7-6eb9e34533fe)

![image](https://github.com/user-attachments/assets/0c2d383e-b6a3-4346-8efa-bf7388fb6b6d)


## Directory to put Binaries (They do NOT go in the normal plugins folder)

**Windows**
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins

**Linux** 
`~/.local/share/gegl-0.4/plug-ins`

**Linux (Flatpak)**
`~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`


Then Restart Gimp and go to Filters>Text Styling and look for "Frozen Gel" or just search for it with /

GIMP 2.10 users will only find this in GEGL Operation drop down list and it will only work on 2.10 if they are using GEGL 0.4.50+, all normal windows builds of GIMP 2.10 ship with GEGL 0.4.48

## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```
## More previews of this based plugin

![image](https://github.com/user-attachments/assets/cca27407-4b56-4dca-b0d3-abab665da333)


