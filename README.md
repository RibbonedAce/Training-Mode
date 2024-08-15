
<img src="Logos/Uncle-Punch-Training-Mode-banner.png" alt="" width="300"/>

# Training Mode - A Melee Modpack for Practicing Tech

Training Mode is a modpack for Super Smash Bros. Melee that aims to streamline practice by providing pre-made individual training scenarios. This mod utilizes the event mode present in Super Smash Bros. Melee and reworks them to focus around specific techniques, complete with automatic savestate functionality to allow for rapid-fire practice. Some events have included video tutorials playable in-game (ISO version only) to teach the player about the tech and how to perform it. In addition to these events, Training Mode also includes onscreen text displays which provide you with information otherwise unavailable to you mid-game.

## How To Compile the ISO on Windows

1. Fizzi36 wrote a program to mass assemble .asm files, it is included in the "Build TM Codeset" folder in this repository. All you need to do is launch the "Build Training Mode Codeset.bat" file and the codes.gct file will be generated and placed in the "Additional ISO Files" folder. This folder contains all the files you need to place into the ISO to run Training Mode minus one file, the Start.dol.

1. The Start.dol is the game's executable and contains copyrighted code, so for that reason I cannot distribute it. However, if you manage to extract the Start.dol file out of your own ISO file you can patch the dol file to load the codeset created prior. Look for the file named "Drag Melee v1.02 Start.dol Here.bat" located in the "Build TM Codeset" folder and drag your extracted 1.02 NTSC Start.dol file onto the .bat file. The modified Start.dol will also be place in the "Additional ISO Files" folder.

1. You can now copy the contents of "Additional ISO Files" to an NTSC 1.02 Melee root folder and rebuild the ISO. I recommend using GCR.

## How To Compile The ISO on MacOS

**Note**: I haven't tested this on Linux.

### Things you'll need

1. Install [brew](https://brew.sh/).

1. Run `brew install go wine xdelta`. This will install `wine` for running GCR, `go` for building our mass assembler, and `xdelta` for patching the .iso.

1. Download a copy of [GCRebuilder](http://www.romhacking.net/utilities/619/).

1. Clone or download the [JLaferri/gecko repo](https://github.com/JLaferri/gecko).

### Before your first build

Note: `$PROJECTROOT` prefers to the root of this project.

1.) `cd` into wherever you downloaded the JLaferri/gecko repo (e.x `~/Downloads/gecko`).

2.) Run `go build`. This will generate a an executable called `gecko`. Place that executable in the `$PROJECTROOT/Build TM Codeset/mac` directory of this project.

3.) You'll need the `Start.dol` file from your Melee iso, this is where `wine` comes in handy. Run `wine $PATH_TO_GCREBUILDER.exe`, GCRebuilder should open its own window, from there you can extract your `Start.dol`. **Your Melee iso md5 hash should be equal to `0e63d4223b01d9aba596259dc155a174`**.

### Building

1. cd into `$PROJECTROOT/Build TM Codeset/mac`, and run `./gecko build`. This will mass assemble the `.asm` files in the `$PROJECTDIR/ASM` directory, you should end up with a `codes.gct` file in `$PROJECTDIR/Additional ISO files`.

    Example output: 
    ```
    Writing to ../../Additional ISO Files//codes.gct...
    Successfuly wrote codes to ../../Additional ISO Files//codes.gct
    Process time was 25.577666254s
    ```

1. Run `./modifyStartDol.sh $PATH_TO_YOUR_START.DOL`, where `$PATH_TO_YOUR_START.DOL` is where the `Start.dol` you extracted is located. This will generate a new `Start.dol` at `$PROJECTROOT/Additional ISO Files/Start.dol`.

1. You can now copy the contents of `$PROJECTROOT/Additional ISO Files` to an NTSC 1.02 Melee root folder and rebuild the ISO, using GCRebuilder as we did in Step 3 in _Before your first build_.

### Common errors

> When running GCRebuilder with wine it crashes with `dyld: Symbol not found: _gliCreateContextWithShared`.

[this](https://stackoverflow.com/questions/22732381/dyld-symbol-not-found-but-nm-reports-otherwise-os-x-update-issue) solves the issue.

> `xdelta3: target window checksum mismatch: XD3_INVALID_INPUT`

Your `Start.dol` you extracted from Melee isn't correct. Make sure your .iso has a md5 hash equal to `0e63d4223b01d9aba596259dc155a174`.

## Modifying Training Mode Events (Windows)
**Note**: These instructions are made for [UnclePunch's version of Training Mode](https://github.com/UnclePunch/Training-Mode/), as is. This repo simplifies things a bit for the _Building your changes_ section.

**Note**: These instructions assume you're running your new ISO on [Dolphin](https://dolphin-emu.org/).

### Things you'll need

1. [A C/C++ compiler](https://visualstudio.microsoft.com/downloads/?q=build+tools). This will help you make your changes in the `.c` files.

1. [GCRebuilder](https://gamebanana.com/tools/6410). This (or a similar application) is required for building your custom ISO.

1. [.NET framework >= v.4.7.2](https://dotnet.microsoft.com/en-us/download). This is required for `MexTK.exe` to function properly when converting your `.c` files to `.dat` outputs.

1. [devkitPro](https://github.com/devkitPro/installer/releases). This is required for `MexTK.exe` to function properly when converting your `.c` files to `.dat` outputs.

1. A working Training Mode ISO. Follow the instructions in the [most recent release](https://github.com/UnclePunch/Training-Mode/releases/tag/v3.0-alpha.7.2). This will provide the `Start.dol` file for us to use in our custom ISO.

### Once: create your Training Mode (TM\) root directory

1. Open GCRebuilder. 

1. Click _Image > Open_, and select your Training Mode ISO. 

1. Right click on `root` (with the disc icon) and click _Export..._, saving your TM root to wherever you can access it. Best practice is to not save it in a place that requires Administrator permissions to modify.

### Performing and building your changes

1. Perform your changes by modifying the `patch\events\{event}\source\{event}.(c|h)` files as you wish.

1. For each event you modified, run the`build.bat` file located in the event directory. If there are issues during the process, it's likely because the C code isn't compiling properly.

1. Copy all of the `patch\events\{event}\output\Ev{evt}.dat` files into the TM `root\TM` directory, replacing files if you need to.

1. Follow the first two steps in _How To Compile the ISO on Windows_, except for a few things:
    
    1. Do not use a vanilla 1.02 `Start.dol` -- use the `Start.dol` in your TM root instead. This will ensure that the game's ID is `GTME01` (the Training Mode ID), instead of `GALE01` (the vanilla SSBM ID). The ISO is required to have the ID `GTME01` so that certain Gecko codes run correctly.

    1. Copy the `Start.dol` file into your TM`root\&&systemdata` directory instead of at the top-level `root`.

    1. Do not copy the `TvXXXX.mth` or `audio\TvXXXX.hps` files into the TM root, as they are not necessary and don't seem to do anything for the Training Mode experience.

1. Open GCRebuilder.

1. Click _Options_, check both _Modify system files_ and _Do not use 'game.toc'_. This will prevent any issues that arise if you decide to add brand new events, as GCRebuilder by default doesn't handle brand new files nicely.

1. Click _Root > Open..._, and open your TM root.

1. Click _Root > Save..._, and choose a location for your new ISO. This will not rebuild the ISO, it will just designate a place for it. If you're running this on Dolphin, you probably want to choose your Dolphin games directory (usually `C:\Users\{User}\Documents\Dolphin Emulator\Games`).

1. Click _Root > Rebuild_. You now have your new ISO and are ready to play.

### Things to verify

1. On the Dolphin home page, right click the column headers at the top, and make sure _File Name_ is checked. This will help you identify your custom ISO over your other SSBM ISOs.

1. On the home page, your new ISO will appear to have the ID `GALE01`, which is incorrect. Right click the entry for your ISO and click _Properties_. 
    
    1. On older versions of Dolphin, go to the _Info_ tab. You'll see an entry that says _Game ID:_. If that entry displays`GTME01`, you're good to go.
    
    1. On newer versions of Dolphin, go to the _Filesystem_ tab. If the root displays `Disc - GTME01`, you're good to go.

1. If you're using [Slippi Launcher](https://slippi.gg/downloads)'s version of Dolphin, go to _Properties_ and navigate to the _Gecko Codes_ tab. Make sure that the codes are the same as they are in your base Training Mode ISO.

### Common pitfalls

> The game launches to a black screen.

The ISO probably has the ID `GALE01` instead of `GTME01`. If this is true, you probably used the `Start.dol` of a vanilla 1.02 SSBM root, and the Gecko codes aren't working properly. Redo the steps in _Performing and building your changes_, making sure to use the `Start.dol` of your TM root.

> The pause menu is blank and I can't do anything with it.

This is the case if you build the current codeset of Training Mode without modifications. You can still LRAS out of the events. If you want to fix this issue, go to each of the `.c` files for the events, and change at the bottom:

```
static EventMenu *Event_Menu = &EvtMenu_Main;
```

to 

```
static EventMenu *Event_Menu = &EvtMenu_Main;
static EventMenu **menu_start = &Event_Menu;
```

and rebuild your changes to a new ISO.
