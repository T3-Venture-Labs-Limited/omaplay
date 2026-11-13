# Security

## Reporting a vulnerability

Please report security problems privately, not in a public issue: use
[GitHub's private vulnerability reporting](https://github.com/T3-Venture-Labs-Limited/omaplay/security/advisories/new)
for this repository. We aim to reply within a week.

## What is in scope

- Anything the omaplay patches add or change: the Omarchy theme reader, the interface changes, the packaging and the
  install scripts.
- The [omaplay bar plugin](https://github.com/T3-Venture-Labs-Limited/omaplay-plugin).

Vulnerabilities in VLC itself (demuxers, decoders, network access and so on) should be reported to VideoLAN, following
[their security policy](https://www.videolan.org/security/). If one affects omaplay's pinned VLC snapshot, please also
tell us, so we can move to a fixed upstream commit.

## Supported versions

Only the latest release is supported. omaplay is built on a pre-release snapshot of VLC 4 and moves to newer snapshots on
demand, so a security fix may arrive as a move to a newer VLC commit.
