# Contributing to omaplay

Thanks for helping. omaplay is a small, experimental project maintained on demand, so focused changes are easiest to
review and land.

## Reporting a bug

Open an issue with the bug report template. The most useful details are:

- `pacman -Q omaplay`, your Omarchy version and your GPU;
- what you did, what you expected and what happened;
- a log: run `omaplay -vv 2>&1 | tee omaplay.log` and reproduce the problem.

If the same problem happens in the official VLC 4 (for example a codec or streaming bug), it belongs upstream at
[VideoLAN's issue tracker](https://code.videolan.org/videolan/vlc/-/issues). omaplay does not change VLC's playback code.

## What fits

- Making the interface follow Omarchy better: theming, fonts, motion, Hyprland behaviour.
- Keyboard and usability improvements that keep every stock VLC feature reachable.
- Packaging, docs and fixes to anything in `patches/`.

Changes to decoding, demuxing or other VLC internals should go to VideoLAN instead: omaplay keeps those identical to
upstream so it stays easy to rebase.

## Making a change

The repository holds VLC as a patch series, not as source. The workflow, in short (details in
[docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)):

```bash
bin/setup                 # clone VLC at the pinned commit into vlc/ and apply the patches as commits
# edit in vlc/, commit on its `omaplay` branch (one topic per commit; the message becomes the patch description)
bin/build                 # incremental build
QT_QPA_PLATFORM=offscreen meson test -C build --suite qt --suite qt+test
bin/parity                # every stock option and hotkey still exists
bin/export-patches        # rewrite patches/ from the branch
```

Then commit `patches/` (and anything else you changed) in this repository and open a pull request. To change an existing
patch rather than add one, use `git commit --fixup` and an autosquash rebase in `vlc/`, so the series stays one commit per
topic.

Please:

- keep each build option defaulting to stock VLC behaviour (see `bin/meson-flags`);
- add or update a test in `vlc/modules/gui/qt/tests/` when you change logic;
- for interface changes, attach a before and after screenshot (`bin/harness` makes clean ones);
- describe what you tested in the pull request.

## Code of conduct

Everyone taking part is expected to follow the [code of conduct](CODE_OF_CONDUCT.md).

## Licence

By contributing you agree that your contribution is licensed under the GNU General Public License, version 2 or later, the
same as omaplay and VLC.
