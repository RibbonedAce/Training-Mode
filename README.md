

<img src="Logos/Uncle-Punch-Training-Mode-banner.png" alt="" width="300"/>

# Training Mode - A Melee Modpack for Practicing Tech

Training Mode is a modpack for Super Smash Bros. Melee that aims to streamline practice by providing pre-made individual training scenarios. This mod utilizes the event mode present in Super Smash Bros. Melee and reworks them to focus around specific techniques, complete with automatic savestate functionality to allow for rapid-fire practice. Some events have included video tutorials playable in-game (ISO version only) to teach the player about the tech and how to perform it. In addition to these events, Training Mode also includes onscreen text displays which provide you with information otherwise unavailable to you mid-game.

## How To Compile the ISO on Windows

1. Fizzi36 wrote a program to mass assemble .asm files, it is included in the "Build TM Codeset" folder in this repository. All you need to do is launch the "Build Training Mode Codeset.bat" file and the codes.gct file will be generated and placed in the "Additional ISO Files" folder. This folder contains all the files you need to place into the ISO to run Training Mode minus one file, the Start.dol.

1. The Start.dol is the game's executable and contains copyrighted code, so for that reason I cannot distribute it. However, if you manage to extract the Start.dol file out of your own ISO file you can patch the dol file to load the codeset created prior. Look for the file named "Build Start.Dol.bat" located in the "Build TM Codeset" folder and drag your extracted 1.02 NTSC Start.dol file onto the .bat file. The modified Start.dol will also be place in the "Additional ISO Files" folder.

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

## Building Training Mode Events (Windows)

**Note**: These instructions are made for [UnclePunch's version of Training Mode](https://github.com/UnclePunch/Training-Mode/), as is. This repo simplifies things a bit for the _Building the ISO_ section.

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

### Building the ISO

1. For each event, run the`build.bat` file located in the event directory. If there are issues during the process, it's likely because the C code isn't compiling properly.

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

### Building training mode events with the batch scripts

**Note**: These instructions assume you're running your new ISO on [Dolphin](https://dolphin-emu.org/).

1. Follow the steps for _Things you'll need_ and _Once: create your Training Mode (TM\) root directory_.

2. Create environment variables, either on your device or on the program that you're using to run the batch scripts, if any. Make sure to surround these paths in double quotes, even if they don't have any spaces.

    1. `TM_GCR_ROOT` = The directory that has your root that you extracted from your TM ISO.
        > `TM_GCR_ROOT="C:\Users\user\Documents\CLionProjects\TrainingMode\gcr\root"`
        
    1. `TM_CGR_EXE` = The executable file that runs GCRebuilder to rebuild your custom ISO.
        > `TM_CGR_EXE="C:\Users\user\Documents\CLionProjects\TrainingMode\gcr\gcr.exe"`
        
    1. `TM_ISO_PATH` = The file path that you want to put your new ISO into. This is usually in the directory that your Dolphin games are kept.
        > `TM_ISO_PATH="C:\Users\user\Documents\Dolphin Emulator\Games\SSBM-TM-Rebuilt.iso"`
        
    1. `TM_DOLPHIN_EXE` = The executable file that starts up Dolphin and uses your new ISO. This can be either a [vanilla version](https://dolphin-emu.org/) or a _Faster Melee_ version such as [Slippi](https://slippi.gg/downloads).
        >  `TM_DOLPHIN_EXE="C:\Users\user\AppData\Roaming\Slippi Launcher\netplay\Slippi Dolphin.exe"`

1. There are 3 main batch files to run:

	1. `Build Everything.bat` will build all events, your `Start.dol`, and your `codes.gct`, and copy everything into the proper path in `Additional ISO Files`.
	
	1. `Rebuild ISO.bat` will move everything from `Additional ISO Files` into your TM root, and use GCRebuilder to build your ISO where you're pointing it to.
	
	1. `Run Dolphin.bat` will start up Dolphin and run your newly created ISO.
	
	1. There is also `Build Rebuild Run.bat`, which will run all of those in sequence, going from code files to a running ISO in a single executable.

### Common pitfalls

> The game launches to a black screen.

The ISO probably has the ID `GALE01` instead of `GTME01`. If this is true, you probably used the `Start.dol` of a vanilla 1.02 SSBM root, and the Gecko codes aren't working properly. Redo the steps in _Building the ISO_, making sure to use the `Start.dol` of your TM root.

> The pause menu is blank and I can't do anything with it.

This is the case if you build the current codeset of Training Mode without modifications. You can still LRAS out of the events. If you want to fix this issue, go to each of the `.c` files for the events, and change at the bottom:

```
static EventMenu *Event_Menu = &EvtMenu_Main;
```

to 

```
static EventMenu *Event_Menu = &EvtMenu_Main;
EventMenu **menu_start = &Event_Menu;
```

and rebuild your changes to a new ISO.

## Creating and Modifying Training Mode Events (Windows)

**Note**: These instructions assume you're running your new ISO on [Dolphin](https://dolphin-emu.org/).

### Things you'll need

1. Everything from the _Building Training Mode Events (Windows)_ section.

1. [HSDRaw](https://github.com/Ploaj/HSDLib). This will let you view and edit your asset's file by modifying the structure and the textures within.

1. [Optional] [DAT Texture Wizard](https://github.com/DRGN-DRC/DAT-Texture-Wizard). HSDRaw doesn't automatically assign types to the objects inside, but DAT Texture Wizard does, so this can help verify that your assets are setup correctly.
 
1. [Optional] A hex editor such as [ImHex](https://imhex.werwolv.net/). Sometimes, you can't make specific changes to the bytes in your assets file in HSDRaw, do this editor can help.

### Adding a new event in the patch files

These instructions are basically a step-by-step process of [this commit](https://github.com/RibbonedAce/Training-Mode/commit/f53874876dbe75500914a8a47584898e2cb6f87e).

We will be adding a new event by copying the Ledgedash event into a new slot on the Events Menu and running it. You don't have to add a new event this way; this is just how I did it.

**Note**: These instructions assume that you are using the Batch files as detailed in the _Building training mode events with the batch scripts_ section.

1. In the `patch\events` directory, copy the `ledgedash` into a new event alias, such as `pivotfsmash`.

1. In the new event directory, change the names of the files in the `assets` and `source` directories to match your new event name. 

    1. For example, change the assets DAT file from `ldshData.dat` to `pfshData.dat`.
   
    1. And change the `ledgedash.(c|h)` files to `pivotfsmash.(c|h)`.

1. In the `build.bat` file, make sure to change any references to the old event into the new event. 

    1. This is also where we will choose the name of the file that will go into the ISO. It makes sense to follow the conventions of the existing files and name it something such as `EvPfsh.dat`. 

    1. The maximum name length of this file name is 20 characters, including the `.dat`.

1. In `pivotfsmash.c`, in the `Event_Init` function, you need to change the reference of `ldshData` to `pfshData` to match your assets file name.

1. Add the event to `events.c`.

    1. Copy the instances of `EventMatchData` and `EventDesc` for the ledgedash event to new objects, renaming them as you wish.

    1. The internals can mostly stay the same, but make sure that `EventDesc.matchData` points to your new `EventMatchData` object, and that `EventDesc.eventFile` is `EvPfsh` to match the ISO file name.

    1. Add your new `EventDesc` instance into one of the event lists down below. The location where you place it will affect the order of the events shown in the Events Menu.

1. Fix `pfshData.dat`.

    1. Open HSDRaw.

    1. Open `pfshData.dat` in the editor.

    1. At the root of the structure shown on the left, change the `ldshData` text to `pfshData`.

    1. Save the assets file.

1. Get GCRebuilder to recognize your new ISO file.

    1. Run the `Build Everything.bat` script. This should place `EvPfsh.dat` into `Additional ISO Files\TM` along with the existing files.

    1. Open GCRebuilder.

    1. Click _Options_, check both _Modify system files_ and _Do not use 'game.toc'_. Unfortuantely, the CLI does not have a way to use these settings, which is why we have to use the application directly.

    1. Rebuild the ISO through the app. Everything should already be in place if the batch scripts were used to generate `EvPfsh.dat`.

1. Run the `Build Rebuild Run.bat` script to test if everything works. You should see your new event on the Events menu, and it should act identically to the ledgedash event.

Now you can modify the event as you wish and use the batch scripts to build, rebuild, and test on Dolphin.

**Note**: You might have noticed references to the events in the `ASM` directory. Those references do not mean anything; we do not need to add anything related to your new event there.

### Modifyng your new event

1. Make the respective changes in `pivotfsmash.(c|h)` and/or `pfshData.dat` as you wish.

1. Follow the instructions in _Building Training Mode Events (Windows)_ if you haven't already, to rebuild the ISO and test your changes.

1. Every time you want to test your changes from now on, all you have to do is run the `Build Rebuild Run.bat` script.

### Things to verify

1. After you build the ISO with your new event, re-open the ISO with GCRebuilder to see if your new event is actually present. If it's not there, your event will fail to load when selected in the game.

### Common pitfalls

> The event does not launch. After I select a stage, the game hangs and I hear the crowd gasp.

This means that the game was not able to find your new `EvPfsh.dat` file in the ISO. Make sure to follow the sub-steps in _Get GCRebuilder to recognize your new ISO file_ correctly.

> The event loads, but the custom HUD does not load, and Dolphin is spitting out invalid read errors at me.

This means that the game was not able to properly load the custom assets in `EvPfsh.dat`. Make sure to follow the sub-steps in _Fix `pfshData.dat`_ correctly.

## Things we're not allowed to do purely in C
 
1. Reference `event_vars` outside of the established event files.

1. Add any public variables/functions in `events.h`.

1. Rearrange fields of event data/assets inner structs.
