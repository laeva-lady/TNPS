# TNPS
get TODOs from files

```
tnps <flags>
```

flags:
- `-f` : select files, since this flags consumes the following arguments, `--` is needed to stop the swallow.
- `-p` : select a pattern
e.g.:
```
$ tnps -p "TODO" -f file1 file2 ...
$ tnps -f file1 file2 ... -- -p "TODO"
```
