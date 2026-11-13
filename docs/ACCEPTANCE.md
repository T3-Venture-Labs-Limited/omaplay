# Acceptance status (v0.1.0)

One row per acceptance scenario of the first release. **Verified** means it was run and observed;
**Partly** says what was and was not covered; **Not verified** means no evidence yet. Themes used: Catppuccin (dark),
Catppuccin Latte (light), Nord and Solitude. Tools: `bin/harness` (cage sandbox), MPRIS, a real Hyprland session on Omarchy 4.5.

## omarchy-shell-plugin

| Scenario | Status | Evidence |
|---|---|---|
| Manifest validates | Verified | `omarchy plugin validate plugin` exits 0 |
| Enable adds widget to bar | Verified | fresh `bin/install-plugin` with no shell restart: enabled, placed after the tray, IPC answers |
| VLC 3 playing alone | Verified | widget status `not running`; VLC 3 stayed `Playing` and untouched |
| omaplay crashes | Verified | player killed with SIGKILL: widget idle within 0.5 s, shell answers ping |
| Title shown during playback | Verified | bar shows the pause glyph and the file name |
| Middle click toggles playback | Not verified | code path exists; no pointer injection into the live bar |
| Seek from panel | Verified | by hand: dragging the panel's seek bar moves playback. Arrow-key seeks measured over MPRIS: +5.1, +5.1, +5.2, -4.9 s for 5 s steps (they used a cached position before the fix in the plugin's second commit) |
| Launch from idle panel | Verified | by hand: "Open omaplay" in the idle panel starts the player |
| Panel keyboard navigation | Verified | by hand: arrows move between controls, Left/Right adjust the sliders, Enter activates, Esc closes |
| Markup-shaped title | Verified | `status` returns `<b>bold & <i>x.mp4` literally; every text is `Text.PlainText` |
| Theme switch | Partly | the app followed a real `omarchy theme set nord` and back; the widget uses the bar's colour tokens but its recolour was not measured |
| Volume step via keybinding | Verified | volume 0 to 0.05 to 0 (5% steps) through `volumeUp`/`volumeDown` |

## omarchy-theme-integration

| Scenario | Status | Evidence |
|---|---|---|
| Interface uses theme accent and background | Verified | Catppuccin `#89b4fa` and Solitude `#798186`, sampled from pixels |
| Light theme | Verified | Catppuccin Latte: light surfaces, dark text. Light-scheme icon variants not separately checked |
| Theme changes during playback | Verified | a real `omarchy theme set nord`: colours changed and restored, playback continued; an open Preferences dialog recolours live too |
| Theme directory replaced through symlink swap | Verified | unit tests (file and directory swaps, twice) and Omarchy's `rm -rf; mv` sequence in the sandbox |
| Low-contrast accent | Partly | unit test for the widget palette and the clamp; the QML palette pass was not measured in the UI |
| Default Omarchy rounding | Verified | fake `hyprctl` returning 0: square corners |
| Rounded windows | Verified | fake `hyprctl` returning 6: panel corners rounded, small controls capped |
| Preferences dialog themed | Verified | themed in dark and light; the dialog code is unchanged so every page is present |
| No Omarchy theme present | Verified | exactly one warning naming the path; `#FFD60A` accent rendered |
| Malformed colour value | Verified | unit test: fallback accent, other keys applied |
| User selects stock Night scheme | Verified | a saved Night index wins over an active light theme and stays persisted |

## player-interface

| Scenario | Status | Evidence |
|---|---|---|
| Parity checklist passes | Verified | `bin/parity`: 2215 entries, 0 stock options or hotkeys missing (`docs/PARITY.md`) |
| Menu action relocated by redesign | Verified | the eight menus' code is identical to stock; the overflow menu lists all eight |
| Command-line options unchanged | Verified | option set identical to the stock baseline; options such as `--start-time` ran |
| Animations disabled | Verified | frame burst: first frame differs 58% with animations, 2.6% without |
| Window under Hyprland | Verified | `hyprctl clients`: class `omaplay`, title "… — omaplay", native Wayland; no window buttons drawn |
| Media library unavailable | Verified | Open File with its six-entry dropdown, and a Recent list |
| Controls auto-hide | Verified | visible at 1.2 s and 2.4 s, hidden at 4.4 s (cursor hiding itself not observable in cage) |
| Controls reappear | Verified | pointer move brought them back |
| Shortcut overlay | Partly | `Shift+/` lists bound entries from the current key configuration; a user-customised binding was not tried |
| Command palette reaches a deep action | Verified | Ctrl+K, "sync", Enter opens the Synchronization tab |
| Stock hotkey works | Verified | Space pauses and resumes |
| Load external subtitle | Partly | `--sub-file` loads and ticks the track; the "Add subtitle file" menu route was not run |
| Reorder queue by keyboard | Verified | Alt+Up/Alt+Down (Ctrl+Up/Down are VLC's volume keys) |
| Unreachable stream | Verified | themed notification and a details dialog naming the URL |
| Recent list after a playlist | Verified | four files played in order over MPRIS `Next`: the Recent list holds each file once, each with its own title (a title used to land on the previous entry) |
| Toolbar editor persists | Verified | by hand: a control dragged in the editor, OK, restart; the change was kept |

## side-by-side-install

| Scenario | Status | Evidence |
|---|---|---|
| Both players installed | Verified | `bin/verify-install` 14 of 14 |
| No library cross-loading | Verified | `/proc/<pid>/maps`: libraries and plugins only from the install prefix |
| VLC 3 settings untouched | Verified | `vlcrc` checksum identical |
| Both players playing | Verified | `playerctl`-equivalent: two MPRIS names; pausing one left the other playing |
| Default player unchanged after install | Verified | `xdg-mime query default video/mp4` is still `vlc.desktop` |
| First run plays video | Verified | software decode on a fresh config, non-black frames |
| User enables hardware decoding | Partly | verified at the config-file level, not through the Preferences dialog |
| Build uses pinned commit | Verified | clean clone applies every patch without fuzz and builds |
| Upstream update is explicit | See task 8.4 | |
| Uninstall | Verified | `pacman -R` of the previous build removed its whole prefix, launcher entry and command; the official VLC 3 kept working; reinstall then passed `bin/verify-install` |

Tested hardware: one x86_64 laptop (Intel + NVIDIA graphics), Omarchy 4.5, Hyprland, Qt 6.12. Other GPUs,
compositors and Omarchy versions are untested. Reports are welcome.
