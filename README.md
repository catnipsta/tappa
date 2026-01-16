# tappa
Scrolling rhythm game for the UNIX terminal.

## Install
```
git clone https://github.com/catnipsta/tappa.git && cd tappa
make
sudo make install
```

## Play a level
```
tappa level.tap
```

## Create a level
```
taprec level.tap (level speed) path/to/song.{mp3,wav,ogg,flac}
```
(level speeds are recommended to be somewhere from 15 to 64)

## Randomize a level
```
taprand level.tap
```

## Extra features
Q to quit.
</br>
Space bar to pause.
</br>
When playing a level, R restarts.
</br>
When recording a level, the / key allows you to seek to a specific time stamp of the level.
