# Maintaining omaplay

omaplay follows VLC 4's development branch, which VideoLAN describes as experimental. Security and bug fixes land
upstream; omaplay only gets them when the pin in `UPSTREAM` is moved. Updates are made on demand.

## Moving to a newer VLC

```bash
bin/update-upstream origin/master     # or a commit id
```

It changes `UPSTREAM` and `patches/` only if every step passes:

1. fetch, then rebase the omaplay commits onto the new commit (a backup branch `omaplay-before-<sha>` is kept);
2. rebuild from scratch;
3. run the Qt tests;
4. regenerate the parity inventory (fails if a stock option or hotkey disappeared);
5. record the new pin and re-export the patches.

Then run the checks that touch what changed (at least `bin/harness start --wizard`, a themed playback, the command palette
and the play queue), bump `pkgver`/`pkgrel` and `_vlc_commit` in `packaging/PKGBUILD`, run `updpkgsums`, and release.
If the rebase conflicts, the script aborts it, lists the files and prints the manual command.
Roll back with `git -C vlc reset --hard omaplay-before-<sha> && bin/setup`.

## Releasing

1. Commit, then tag `vX.Y.Z` and push the tag; GitHub serves `archive/refs/tags/vX.Y.Z.tar.gz`, which the PKGBUILD downloads.
2. In `packaging/`, set `pkgver`, run `updpkgsums` and `makepkg --printsrcinfo > .SRCINFO`.
3. Publish `PKGBUILD`, `.SRCINFO` and `omaplay.install` to the AUR package repository.

## Things that will bite you

- **The meson build is incomplete upstream.** It never defines `HAVE_DBUS` for the core, which removes `--dbus` and
  `--one-instance`; our `src/meson.build` change adds it. If upstream fixes this, drop that hunk.
- **Do not make `dbus` default-on in the configuration.** The `vlc-preparser` helper processes then each register an MPRIS
  name. The `vlc` binary passes `--dbus` itself.
- **New colour schemes break the first-run wizard.** Its colour page asserts one button per scheme. omaplay skips that page;
  keep it that way, or the app aborts when the user clicks Next.
- **Two palettes.** `VLCStyle.darkPalette` paints the controls over video and must follow the Omarchy scheme too.
- **`currentChanged` is silent when the saved colour scheme equals the default**, so `MainCtx` applies the scheme once at
  start-up. Remove that call and the widget palette never applies.
- **Omarchy switches themes by replacing directories** (`rm -rf current/theme; mv next-theme current/theme`). The theme
  watcher re-arms after every event and watches the nearest existing ancestors. Test fixtures must not sit directly in
  `/tmp`, where unrelated activity triggers reloads and hides missing re-arming.
- **Patches carry binary files** (icons). Apply them with `git apply` or `git am`, never `patch(1)`.
- **`makepkg -C`.** A reused source tree applies the patches twice.
- **RPATH.** The package needs both `/opt/omaplay/lib` and `/opt/omaplay/lib/vlc` (plugins link `libvlc_xcb_events.so`).
- **Options that do not exist in VLC 4:** `--avcodec-hw` (use `--hw-dec`/`--no-hw-dec`), `--save-config`
  (`--reset-config`), `--no-one-instance`.
- **Never `pkill -x vlc`** while testing: it also kills the official VLC.

## Known upstream issues at the current pin

- `PlayerView.qml:651: TypeError: Property 'scheduleUpdate' of object DualKawaseBlur` is logged at start-up (harmless).
- The media library cannot be built: VLC wants libmedialibrary `>= 0.14, < 0.15` while that project's master is 0.15.
  omaplay therefore ships without it and keeps its own recent-media list.
