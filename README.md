# GD Nova v0.4 — Experimental Auto Bot

Geode mod for Geometry Dash 2.2081 (Windows). The existing Midnight menu now has an AUTO BOT toggle.

## What this prototype actually does

- Drives player 1 jump input in normal gameplay using 120 ms action slots.
- After each level reset, compares the farthest progress of the previous attempt with its previous best.
- Retains the best action sequence and mutates jump choices around the last reached time slot.
- Displays bot attempt count and best progress in the GD Nova HUD.
- Saves the bot-enabled preference using Geode settings (disabled by default).

This is a simple evolutionary / trial-and-error controller, not a neural network, vision system, or reliable general solver. It does not recognize obstacles, handle level mode switches, synchronize with high precision frame-by-frame, or guarantee progress. It does not intentionally bypass collisions. Its learned sequence is in-memory for the current level session and resets when you leave.

## How to build

Open GitHub Actions > Build GD Nova (Windows) and download GD-Nova-Windows from a successful run. If the action fails, view the Build mod logs.

**Compatibility:** Target GD 2.2081 with Geode SDK 5.11.2. Compatibility with GD 2.281 is unverified. Test only on a suitable version, preferably on your own levels, and do not submit assisted clears as legitimate records.
