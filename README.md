# AsyncMapFramework

## Sinopsis

AsyncMapFramework is a tool for VR Games/Apps that allows easy LevelStreaming while VR User is active without Game Locking.

## How to use

Create a LevelStreaming with at least 2 Maps (One of them as a Loading map)

- Add "AsyncMapGameModeHelperComponent" to the GameMode Class where You are going to use in the project and change the "LoadingLevelName" property to the Loading Level Name which you wish to use. (By Default: "Loading")

- Add interface "AsyncGameModeComponentInterface" To the Game Mode and call the same named function in the AsyncMapGameModeHelperComponent.

- Add Interface "AsyncPlayerControllerInterface" to your PlayerController and implement `TeleportToPlayerStart(FName LevelName);` :
  - For your Teleport Behaviour. (You can use the LevelName as a way of searching a PlayerStartTag to Teleport it)
    - TODO: Could be released A Standard PlayerStart Teleport Function Component for this project in the future.

  - To use CustomLocation Loading Mode, `TeleportToCustomLocation()`  should be implemented.  
    - This function was thought to load customlocations from a savedata which was loaded/located in PlayerController.

    - These functions in AsyncPlayerController Interface could help to implement it:
`LoadSaveData(FTransform LastLoadedMap, TMap<EEvents,bool> & EventsEnabled);`  and
`SaveData(FName LastLoadedMap, ELoadingMode LoadingMode);`

      - (These may be will be replaced since It was thought for a single project, but they are hard dependencies which It requires some refactoring in my projects)

If you want to have custom behaviour for Maps (When It's loaded or activated) Add MapInterface to your LevelScriptingActors.
`SetLoadingWidgetVisibility` is thought to remove loading widgets/indicators while playing some cinematic video

To load a map, Call "LoadMap" from AsyncGameModeComponentInterface using the Unreal standard GetGameMode.

It exists 5 Modes:
- Startup,
- Regular,
- Prologue,
- MainMenu,
- CustomLocation.

The three first Are just it's just reporting duty (which could be useful for SetPrologueMode in MapInterface for different map Behaviour),  
MainMenu Changes the Pawn If The Main Game pawn or some function/component) are not suitable for a Main menu Pawn.  

You can determine their behaviour in ReSpawn(bool MainMenu) function in AsyncPlayerControllerInterface.
