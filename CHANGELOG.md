# Changelog

All notable changes to omaplay. Versions follow [semantic versioning](https://semver.org); the VLC snapshot each release
is built on is recorded in [`UPSTREAM`](UPSTREAM).

## Unreleased

- Interim fix for the picture staying frozen after scrubbing while paused and pressing play (VLC issue #30251), using two
  upstream commits. Files without an audio track are still affected; see `docs/MAINTAINING.md`.

## 0.1.0 (2026-11-13)

First public release, built on VLC 4 at commit `6b5f85b1d1bb` (2026-11-11).

- Follows the active Omarchy theme live, light themes included, with readable contrast enforced; the classic dialogs are
  themed too.
- Uses the Omarchy font and Hyprland's corner radius; ease-out motion that follows `omarchy toggle animations`.
- Command palette (`Ctrl+K`) over every dialog and VLC action, and a shortcut overlay (`?`).
- Play-queue reordering with `Alt+Up` / `Alt+Down`.
- Recent files on the home screen without the media library.
- Controls auto-hide after 3 seconds.
- Software decoding by default.
- Installs beside the official `vlc` under `/opt/omaplay`, with its own settings directory, MPRIS name and desktop entry.
- The media library is not included.
