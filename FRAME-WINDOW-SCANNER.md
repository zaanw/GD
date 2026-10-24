# Real Frame Window Scanner build (upstream)

This repository includes a separate manual GitHub Actions workflow that builds the original **Frame Inspector** by lightningstudiosx.

- Original source and author: https://github.com/lightningstudiosx/frame_inspector
- Workflow: `.github/workflows/build-real-frame-inspector.yml`
- This is **not** a feature of our unfinished `nova-frame-counter` mod, and is **not** rebranded as GD Nova.
- The workflow checks out the upstream project and builds it unchanged. No upstream source is copied into this repository.
- Target is Geometry Dash 2.2081; GD 2.281 support is unverified.
- After successful build, download `Frame-Inspector-Windows-Upstream` artifact and extract `kai.frame-inspector.geode` to your `Geometry Dash/geode/mods` folder.
- In game, import a known successful macro, configure TPS, select Scan Level, and replay results to view measured frame windows. Do not mistake frame-window measurement for live estimation without a successful macro.

Build and runtime success are not yet verified by this repository.
