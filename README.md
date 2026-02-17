```
Support String::x86_64::Linux
```

![demo](./asset/pman_demo.gif)

### pman

A very simple notcurses based TUI for personal project management, locally. Not meant for production usage. Written in C++. It simply keeps tracks of project directories, descriptions and status along with any relevant research links for that project. 

### Note

Project identity is defined by (name, proj_path).

The only CLI options are `--help` and `pman foo`, where the latter is about opening the project directory with `$EDITOR` if it supports directory based CLI invocation.

### Usage

Launch with `./pman` command, press `n` to create a project. 

### Building

```bash
mkdir build && cd build
cmake ..
make
```

### LICENSE

MIT






