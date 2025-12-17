# tappa
Simple rhythm game for the UNIX terminal.

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
taprec (levelfilename.tap) (level speed) (path/to/song.{mp3,wav,ogg,flac})
```
(level speeds are reccommended to be somewhere from 10 to 64)

## Extra features
Q quits the level.
</br>
Space bar pauses the level.
</br>
When playing a level, R restarts.
</br>
When recording a level, the / key allows you to seek to a specific time stamp of the level.
