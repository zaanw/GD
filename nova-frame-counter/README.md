# Nova Frame Counter v0.1

A **separate** Geode mod for Geometry Dash on Windows, kept independent of GD Nova and Nova Vision Bot.

During a level, it displays the count of PlayLayer update callbacks since the last restart, and an approximate FPS calculated from the update delta. It resets the count when the level restarts.

**Accuracy note:** this is a simulation-update/frame counter, *not* a reliable count of actual display/GPU frames, and the shown FPS is an approximation of update frequency rather than a measured presentation FPS. It is suitable as a simple visual counter, not for frame-perfect macro verification.

Controls: Geode -> Mods -> Nova Frame Counter -> Settings:
- Show frame counter
- Show estimated FPS
- Text scale

Build: Actions -> Build Nova Frame Counter (Windows), download the Nova-Frame-Counter-Windows artifact if successful, extract the .geode file, place it in Geometry Dash/geode/mods.

Target: GD 2.2081 + Geode 5.11.2. Compatibility with GD 2.281 is not verified.
