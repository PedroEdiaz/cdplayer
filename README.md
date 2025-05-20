# cdplayer — Minimal CDDA Player for Linux

**`cdplayer`** is a minimal CDDA (Compact Disc Digital Audio) player for Linux.
It plays audio CDs directly from `/dev/sr0` (or other CD devices) with almost
no dependencies — relying only on [**tinyalsa**](https://github.com/tinyalsa/tinyalsa).


## Features

- Plays standard audio CDs via CD-ROM device
- Controlled using UNIX signals (no GUI or TUI)
- Depends only on `tinyalsa` for audio playback
- Extremely small and hackable C codebase

## Build
To compile the program run `make` or:
```sh
cc -Itinyalsa/include -o cdplayer cdplayer.c tinyalsa/src/pcm.c tinyalsa/src/pcm_hw.c tinyalsa/src/snd_card_plugin.c
```

## Usage
```sh
./cdplayer cdrom [card=0] [device=0]
```

- `cdrom` — CD-ROM device like `/dev/sr0`
- `card` — ALSA sound card number (default: 0)
- `device` — ALSA PCM device number (default: 0)

## Controls (via `kill`)
You can control playback using UNIX signals:

Signal|Action
------|-----
`SIGUSR1`|Seek forward 1 minute
`SIGUSR2`|Seek backward 1 minute
`SIGSTOP`| Pause playback
`SIGCONT`| Resume playback


## Licence
0BSD – Zero Clause BSD License. See [LICENSE](./LICENSE) for details.
Feel free to use, modify, and share freely — no restrictions.

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests.
