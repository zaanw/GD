# Nova Vision Bot v0.1 (separate Geode mod)

Independent Geode mod **zaan.nova-vision**. The existing GD Nova project remains unchanged.

## How it works

Uses the game's own positions and collision bounds, rather than screenshots. It records known spike/block object IDs when the level adds them, then inspects nearby hitboxes each update. For supported cube levels it predicts an approximate jump point based on current forward speed and taps jump.

This is a **heuristic vision prototype**, not an AI trained on levels. It cannot guarantee a clear. Unknown hazards, slopes, gravity changes, teleporters, dual mode, platforms, transformed obstacles and fast transitions are not solved. It only scans a subset of vanilla object IDs. Test on simple, self-made cube levels.

## Controls

Geode mod settings -> Nova Vision Bot -> Enable vision bot. Off by default. Additional controls: scan distance and scanner HUD.

## Build

The independent workflow is in `.github/workflows/build-vision.yml`. It builds the contents of this subfolder as a separate project on Windows.

Target: Geometry Dash **2.2081** + Geode 5.11.2. Geometry Dash 2.281 compatibility unverified. Do not treat assisted runs as legitimate records.
