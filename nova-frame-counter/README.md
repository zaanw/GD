# Nova Frame Counter v0.2

This is a **separate** Geode mod inside the `zaanw/GD` repository, targeting Geometry Dash **2.2081** / Geode 5.11.2.

## Implemented
- HUD counting actual first-player jump press/release events observed by the game
- Visual DOWN / UP markers as events happen
- Optional PlayLayer update counter (TICKS)
- Estimated update frequency (UPS)
- Configurable text scale and visibility; all toggles stored by Geode
- Counters reset when the level restarts

## Not yet implemented
**Accurate 1f/2f/3f frame window detection is not implemented.** It requires importing a deterministic, successful input replay, shifting each press/release backward and forward frame-by-frame and replaying the simulation to determine safe timings. A click's spacing alone is not enough to infer its frame window. The current indicators deliberately do *not* claim 1f/2f measurements.

## Build and install
Open GitHub Actions > Build Nova Frame Counter (Windows). Download the `Nova-Frame-Counter-Windows` artifact from a *successful* build and extract the .geode into Geometry Dash/geode/mods.

The build and in-game behavior have **not been independently verified**. The mod is targeted at GD 2.2081; do not assume compatibility with GD 2.281.

## Reference implementations
- https://github.com/hyper-5/frame-window-counter
- https://github.com/lightningstudiosx/frame_inspector
These implement much more comprehensive real frame window functionality. Nova Frame Counter is not a copy of either project.
