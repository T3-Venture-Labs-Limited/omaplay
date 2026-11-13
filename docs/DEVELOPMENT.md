# Developing omaplay

omaplay is VLC 4 (one pinned upstream commit, recorded in `UPSTREAM`) plus an ordered patch series in `patches/`.
The upstream checkout in `vlc/` is a build input and is not committed.

## Requirements

Arch Linux (Omarchy) with the VLC build dependencies. The list in `packaging/PKGBUILD` is complete; for a quick start:

```bash
sudo pacman -S --needed base-devel git meson ninja ccache qt6-base qt6-declarative qt6-svg qt6-shadertools \
  qt6-tools qt6-wayland vulkan-headers ffnvcodec-headers wayland-protocols ffmpeg lua chromaprint
```

`qsb` (from `qt6-shadertools`) is required; the Qt interface does not build without it.
For the screenshot sandbox you also need `cage`, `grim` and `wtype`.

## Build and run

```bash
bin/setup                    # clone upstream into vlc/, check out UPSTREAM, apply patches/ on branch omaplay
bin/build                    # meson + ninja into build/ (about 3 minutes on 32 cores, seconds when incremental)
```

Run the in-tree build without installing anything, with throwaway settings:

```bash
export VLC_LIB_PATH=$PWD/build/modules VLC_PLUGIN_PATH=$PWD/build/modules QT_QPA_PLATFORM=wayland
XDG_CONFIG_HOME=/tmp/omaplay-try build/bin/vlc-static "$(bin/test-clip)"
```

`vlc-static` plus `VLC_LIB_PATH` is how VLC runs from a build tree; the external preparser does not start otherwise.

## Making a change

1. Edit in `vlc/` and commit on the `omaplay` branch there (one topic per commit; messages are the patch descriptions).
2. `bin/build`, then test (below).
3. `bin/export-patches` rewrites `patches/` from the branch. Commit `patches/` in this repository.

The build options that turn VLC into omaplay are listed in `bin/meson-flags` and are shared by `bin/build` and the
PKGBUILD. Every option defaults to stock VLC behaviour, so the patches are easy to reason about.
A new option needs an entry in `meson_options.txt`, a `cdata.set*` in `meson.build` and a `#mesondefine` in
`config.h.meson`; without the last one it silently never reaches the code.

## Tests

```bash
QT_QPA_PLATFORM=offscreen meson test -C build --suite qt --suite qt+test   # upstream's Qt tests plus ours
bin/parity                                                                 # every stock option and hotkey still exists
```

Our unit tests: `test_qt_omarchytheme` (theme parsing, live reloads across file and directory swaps, contrast),
`test_qt_color_scheme_model`, `test_qt_recent_media`, `test_qt_fuzzy_match`.

`bin/parity` inventories about 2200 entry points (hotkeys, options, menus, control-bar controls, dialogs, QML files) from
the pinned source, writes `docs/PARITY.md`, and exits 1 if any stock option or hotkey is missing from the build.

## The screenshot sandbox

`bin/harness` runs the build inside `cage`, a kiosk compositor with a headless screen. Screenshots are clean and keystrokes
and clicks can never reach your desktop. Settings and the Omarchy theme are fresh on every start.

```bash
bin/harness start --theme catppuccin -- "$(bin/test-clip)"
bin/harness key ctrl+k s y n c Return      # keys, wtype names
bin/harness click 640 692                  # pointer, on a 1280x720 screen (tools/vptr.c, wlr-virtual-pointer)
bin/harness theme catppuccin-latte         # switch theme the way `omarchy theme set` does
bin/harness shot palette                   # build/shots/palette.png
bin/harness stop
```

Use `--wizard` to show the first-run wizard, `--build DIR` to run another build (for example a stock one for comparison),
and `KEEP_STATE=1` to keep settings from the previous run. After any QML change, look for `ReferenceError` or `TypeError` in
`/tmp/omaplay-h/cage.log`: missing imports only show up at run time.

### README screenshots

The README images were taken at 2x on a real Omarchy desktop without disturbing it: a temporary headless Hyprland
monitor placed away from the real ones (`hyprctl output create headless OMASHOTS`, then
`hl.monitor({ output = "OMASHOTS", mode = "2560x1600@60", position = "0x10000", scale = 2 })`), a temporary window rule
with `no_initial_focus` and `focus_on_activate = false` so the player never takes focus, keys sent to the window with
`hl.dsp.send_key_state({ ..., window = "address:..." })`, and `grim -o OMASHOTS`. The player ran with throwaway
`XDG_*_HOME` directories and its own copy of the theme, so themes could be switched without touching the desktop.
Remove the monitor with `hyprctl output remove OMASHOTS` afterwards.

## Packaging

```bash
bin/install                  # build the Arch package from HEAD exactly as a release would, and install it
bin/install --no-install     # only build it
bin/verify-install           # check the installed package sits beside vlc without touching it
```
