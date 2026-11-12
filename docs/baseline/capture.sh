#!/usr/bin/env bash
# Capture the keyboard-reachable surfaces of a build into build/shots/<prefix>-*.png
#   docs/baseline/capture.sh <build-dir> <prefix> [theme]
set -euo pipefail
cd "$(dirname "$0")/../.."
build=$1; prefix=$2; theme=${3:-}
clip=$(bin/test-clip)
targs=(); [[ -n $theme ]] && targs=(--theme "$theme")
shot() { bin/harness shot "$prefix-$1" >/dev/null; echo "  $1"; }

echo "== idle main window"
bin/harness start --build "$build" "${targs[@]}" >/dev/null
shot home
# pointer-driven main-window surfaces (coordinates for a 1280x720 screen)
bin/harness click 40 123; sleep 2; shot browse
bin/harness click 40 158; sleep 2; shot discover
bin/harness click 1255 50; sleep 1.5; shot menu-overflow; bin/harness key Escape; sleep 0.5
bin/harness click 40 123; sleep 0.5; bin/harness click 40 88; sleep 1; bin/harness click 1210 50; sleep 1.5; shot queue-panel-idle; bin/harness click 1210 50; sleep 0.5
for d in "prefs:ctrl+p" "effects:ctrl+e" "messages:ctrl+m" "mediainfo:ctrl+i" "open:ctrl+o" "network:ctrl+n" "convert:ctrl+r"; do
  name=${d%%:*}; key=${d#*:}
  bin/harness key "$key"; sleep 2.5; shot "dlg-$name"; bin/harness key Escape; sleep 1
done
echo "== player view"
bin/harness start --build "$build" "${targs[@]}" -- --qt-pin-controls "$clip" >/dev/null
sleep 3; shot player
bin/harness key ctrl+l; sleep 1.5; shot player-queue; bin/harness key ctrl+l; sleep 1
bin/harness click 39 692; sleep 1.5; shot player-tracks-menu; bin/harness key Escape; sleep 0.5
bin/harness click 1045 692; sleep 1.2; shot player-volume; bin/harness key Escape; sleep 0.5
bin/harness key space; sleep 1; shot player-paused; bin/harness key space
bin/harness key F11; sleep 2; shot player-fullscreen; bin/harness key F11
bin/harness stop
