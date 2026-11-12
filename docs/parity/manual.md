Checked by hand with `bin/harness` (cage sandbox) and MPRIS, for the files `tools/parity.py` flags as changed.

| File | What changed | Evidence |
|---|---|---|
| `PlayButton.qml` | flat accent ring, theme disc and glyph | clicking it pauses and resumes (MPRIS `Paused` then `Playing`) |
| `NoMedialibHome.qml` | the cone is replaced; a Recent list is added | Open File opens the native chooser; the dropdown lists all six stock entries; a recent entry plays |
| `BannerCone.qml` | rewritten as a play mark, same name and API | renders in both top bars and the home screen |
| `VLCStyle.qml` | radius, duration and palette tokens | every surface in the sweep renders; no `ReferenceError` or `TypeError` in the log other than upstream's known `DualKawaseBlur` warning |
| `ModalDialog.qml` | corner radius token only | not exercised on its own |
| `FloatingNotification.qml` | no longer scales from zero, fades instead | **not exercised**: the notification was not triggered in the sandbox (no audio device, so no volume notification) |
| `qt.cpp`, `help.cpp`, `about.ui`, `systray.cpp`, `preferences.cpp`, `mainctx.cpp` | product name, icons, About colours | window titles read `omaplay`; About keeps VLC's description, authors, licence and credits under "based on VLC media player by VideoLAN" |
| `firstrunwizard.cpp` | colour page skipped; no CSD promise | the wizard was clicked through to the end with no abort |
| `PlaylistPane.qml` | Alt+Up / Alt+Down reorder | moves verified over MPRIS; stops at both ends |
| `ActionOverlay.qml`, `MainInterface.qml` | command palette and shortcut overlay | Ctrl+K, filter, Enter opens the dialog; Esc closes; playback unaffected |
| `messages.cpp` | readable severity colours on dark themes | error details legible on a dark theme |
| 24 other QML files | easing curves only (ease-out) | loaded without errors in the sweep of 19 surfaces |

Intentional default changes (the option still exists and can be changed in Preferences):

| Option | Stock | omaplay | Why |
|---|---|---|---|
| `dec-dev` (Hardware-accelerated decoding) | Automatic | Disable | software decoding avoids broken hardware decoders (black picture on some Intel drivers); switch it to Automatic in Preferences > Input / Codecs |
| `mouse-hide-timeout` | 1000 ms | 3000 ms | spec: controls and cursor hide after 3 s |
| colour scheme (saved setting) | System | Omarchy | follows the desktop theme; System, Day and Night remain selectable |
| first-run wizard | colour page, may enable client-side decoration | colour page skipped, never enables CSD | the compositor owns the window frame |

Options added to match a stock autotools build (the meson build lacked them): `--dbus`, `--one-instance`,
`--one-instance-when-started-from-file`, `--playlist-enqueue`, `--started-from-file`.

Stock surfaces with no code change (menus, dialogs, toolbar editor, play queue, mini player, network views) were captured themed
in `docs/baseline` comparisons; their behaviour is the unchanged stock code.
