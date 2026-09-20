# UE5-character-input-framework

A minimal third-person character for Unreal Engine 5.7 (C++), driven by the **Enhanced Input** system: move, look and jump are defined as data assets, bound in a single C++ character class, and wired into a playable Blueprint, game mode and level.

Project / module name: `Vesper`. Engine association: 5.7.

---

## What is in the repo

| Path | Contents |
|---|---|
| `Source/Vesper/Character/Playable/ExplorationCharacter.{h,cpp}` | `AExplorationCharacter` (derives from `ACharacter`): spring arm + camera, Enhanced Input bindings, `Move` / `Look` / `Jump` |
| `Content/Input/IMC_ExplorationMappingContext` | Input Mapping Context (keyboard and mouse) |
| `Content/Input/Actions/` | `IA_Move` (Axis2D, Down trigger), `IA_Look` (Axis2D), `IA_Jump` (Boolean) |
| `Content/Blueprints/BP_ExplorationCharacter` | Blueprint child of `AExplorationCharacter`; assigns the mapping context and the three actions, overrides `JumpZVelocity` |
| `Content/Blueprints/BP_ExplorationGameMode` | `AGameModeBase` child; `DefaultPawnClass = BP_ExplorationCharacter` |
| `Content/Maps/World` | World Partition level that uses `BP_ExplorationGameMode` through its World Settings |
| `Config/` | Engine, input and game settings |

### Data flow

```
Key / mouse
  -> IMC_ExplorationMappingContext   (key -> action, modifiers, triggers)
  -> IA_Move / IA_Look / IA_Jump     (typed value + trigger event)
  -> UEnhancedInputComponent binding in AExplorationCharacter
  -> AddMovementInput / AddControllerYawInput / AddControllerPitchInput / ACharacter::Jump
  -> UCharacterMovementComponent     (integrates movement with DeltaTime)
```

Controls, as configured in the mapping context: W/A/S/D move, mouse look, Space jumps.

---

## Requirements and getting started

- Windows, Unreal Engine 5.7
- Visual Studio with the components listed in `.vsconfig` (Game development with C++, MSVC v14.38 and v14.44 toolsets, Windows 11 SDK 22621, Clang tools)
- A DirectX 12 GPU with Shader Model 6. The project settings enable Lumen (global illumination and reflections), Substrate, virtual shadow maps and ray tracing, so it targets a high-end desktop GPU.

1. Right-click `Vesper.uproject` -> Generate Visual Studio project files.
2. Build the `VesperEditor` target (Development Editor, Win64) and open the project.
3. Open `Content/Maps/World` manually and press Play. `GameDefaultMap` in `Config/DefaultEngine.ini` still points at the engine template (`/Engine/Maps/Templates/OpenWorld`) and no editor startup map is committed.

---

## Engineering notes

### 1. Frame rate independence
- The character only sends *intent*: `AddMovementInput` takes a direction and a scale in the range [-1, 1]. Integration with `DeltaTime`, acceleration, friction and collision happen inside `UCharacterMovementComponent`, so movement speed does not depend on frame rate.
- `Look` adds the input value straight to the controller rotation with no `DeltaTime` scaling. That is correct for mouse input, which is already a per-frame displacement. It would be wrong for an analog stick, which is a rate and must be multiplied by `DeltaTime` (and by a degrees-per-second constant). Only keyboard and mouse are mapped today, so this is a constraint to remember when gamepad support is added.
- `bEnableMouseSmoothing=True` is set in the project input settings (the engine default). Smoothing depends on frame time, so whether it affects `Mouse2D` under Enhanced Input is worth checking if the look feel needs to be repeatable.

### 2. Input math
- `Move` builds its basis from the **yaw of the control rotation only** (pitch and roll are zeroed). Looking up or down therefore does not tilt the movement direction, and the character stays in the horizontal plane.
- `AddMovementInput` is called twice per frame (forward and right). The movement component sums the accumulated input and clamps its length to 1 (a clamp, not a normalization), so diagonal movement is not faster and partial analog input keeps its magnitude.
- The same `FRotationMatrix` is built twice per call (once per axis). It is cheap, but computing the basis once is the cleaner form.
- In UE5, `FVector`, `FRotator` and `FVector2D` hold 64-bit doubles (Large World Coordinates), while some engine APIs and stored assets still use floats. Conversions at these boundaries are where precision surprises come from.
- Mouse sensitivity is `0.07` and gamepad dead zones are `0.25` (engine defaults, stored in `Config/DefaultInput.ini`). With FOV scaling on (`FOVScale = 1/90`), look speed is scaled relative to a 90 degree field of view.

### 3. Event semantics
- All three actions are bound to `ETriggerEvent::Triggered`, which fires **every frame while the input is active**. That is what continuous movement and look need.
- For `Jump` it means the jump call repeats while Space is held, and there is no `StopJumping` binding. The character can jump again immediately after landing. The usual pattern is `Started` -> `Jump` and `Completed` -> `StopJumping`.
- `AExplorationCharacter::Jump()` only forwards to `ACharacter::Jump()`, so the override adds nothing; binding `&ACharacter::Jump` directly is equivalent.

### 4. Object lifetime and failure behaviour
- The asset references (`InputMapping`, `MoveAction`, `LookAction`, `JumpAction`) are `UPROPERTY` members, so they are tracked by the garbage collector and visible to the Blueprint.
- They are not checked before use. If one is left unassigned on the Blueprint, the problem shows up at runtime, not at edit time. An early-out or `ensureMsgf` on each asset would make the failure explicit.
- `CastChecked<UEnhancedInputComponent>` fails fast if the input component class is wrong. `DefaultInputComponentClass` is set to `EnhancedInputComponent` in `Config/DefaultInput.ini`, so the assumption is consistent with the configuration.
- Header pointers are raw pointers. Since UE 5.0 the recommended type for `UPROPERTY` object references in headers is `TObjectPtr<>`.

### 5. Per-frame cost
- `PrimaryActorTick.bCanEverTick = true`, but `Tick` and `BeginPlay` are empty. An actor that can tick is registered with the tick scheduler and pays a per-frame cost for no work. Setting `bCanEverTick = false` removes it, because all behaviour here is event-driven.

### 6. Build configuration
- Both targets pin build settings (`BuildSettingsVersion.V6`, `EngineIncludeOrderVersion.Unreal5_7`) and `.uproject` pins the engine association to 5.7, so the build is tied to one engine version on purpose.
- `EnhancedInput` is a **public** dependency of the module because `FInputActionValue` appears in a public header (`ExplorationCharacter.h`). If it were only used in the `.cpp`, it could be private.
- `ExplorationCharacter.cpp` includes `SpringArmComponent.h` and `CharacterMovementComponent.h` twice.

### 7. Repository and assets
- The level uses World Partition with one-file-per-actor storage: 138 small files under `Content/__ExternalActors__` and 5 under `Content/__ExternalObjects__`, all binary `.uasset`.
- There is no `.gitattributes`, so binary assets are stored in plain git (no Git LFS). They cannot be diffed or merged, and every change adds a full copy to the history.
- `Config/DefaultEngine.ini` contains an auto-generated `SecurityToken` for the Android File Server plugin. It is low risk, but a token does not belong in a public repository; removing the section (or regenerating the token) is the safe choice.

---

## Status and known gaps

- Implemented: Enhanced Input setup in C++, move / look / jump, spring-arm camera, playable Blueprint, game mode, test level.
- The character never rotates: `bUseControllerRotationYaw` and `bOrientRotationToMovement` are both `false`, so it moves in any direction without turning to face it. A facing policy is not implemented yet.
- The mapping context is added inside `SetupPlayerInputComponent`; if the pawn is possessed by a different controller later, the context has to be managed explicitly.
- No automated tests.
- The binary assets were inspected as files, not opened in the editor.

## Roadmap

1. Bind jump as `Started` -> `Jump` and `Completed` -> `StopJumping`; remove the empty `Jump` override.
2. Set `bCanEverTick = false`, remove empty overrides and duplicate includes.
3. Validate the input assets (`ensureMsgf` or early-out) and switch header pointers to `TObjectPtr<>`.
4. Add the mapping context from a controller-change hook (for example `NotifyControllerChanged`) and remove it from the old controller.
5. Move the input math (control yaw + 2D input -> world-space direction) into a free function and cover it with UE Automation tests (`IMPLEMENT_SIMPLE_AUTOMATION_TEST`). It is pure math and needs no world.
6. Separate mouse look (delta) from stick look (rate x `DeltaTime`) and expose sensitivity as a `UPROPERTY` or data asset.
7. Set `GameDefaultMap` and the default game mode in project settings so a fresh clone runs the project's own level.
8. Add `.gitattributes` for Git LFS (`*.uasset`, `*.umap`) and remove the committed token.
9. Measure before optimizing: capture frame time with `stat unit` and Unreal Insights, and report average and p99 frame time.
