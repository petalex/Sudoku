# Sudoku
**Sudoku** is a console-level graphics interactive game based on world-wide popular [Japanese puzzle][1].<br />
This is a **team** project done as part of [Practical Programming][2] elective course in second semester of my bachelor studies.

## Installation
Main project depencendy is [Curses][3] library used for console graphics. Project installation slightly differs depending on which operating system you are using and is explained further bellow.

### Linux-based OS
First, you must install mentioned dependency which is `ncurses` packege on Linux distributions.<br />
Command for installing `ncurses` package sligthly varies depending on specific distribution:

#### Debian / Ubuntu OS
```bash
$ sudo apt-get install libncurses5-dev libncursesw5-dev
```

#### CentOS / RHEL / Scientific Linux 6.x/7.x+ and Fedora Linux 21 or older
```bash
$ sudo yum install ncurses-devel
```

#### Fedora Linux 22.x+
```bash
$ sudo dnf install ncurses-devel
```
<br />

After successful pacakge instalation, only thing left to do is to build the project using `Make` and run the game:
```bash
$ cd /sudoku-directory-path
$ make build
$ ./bin/sudoku
```

>*`make` and `gcc` packages must be installed before project compilation, but are usually pre-installed with the distribution.*

### Windows OS
Prerequisites for Windows OS are C compiler for Windows and `Make`.<br />
Preferred option is to install `MinGW` package which contains `gcc` compiler (more information can be found [here][4]).<br />
Also, `Make` can be installed by downloading setup program from its [site][5] and adding it to `path` variable.

After successful prerequisites' installation, only thing left to do is to build the project using `make` and run the game:
```console
> cd /sudoku-directory-path
> make build-windows
> .\bin\sudoku.exe
```
>*`make build` rule builds project using `gcc` compiler (you must adjust **makefile** or build project manually if you want to use **different** compiler or do **not** want to install **make**).*

## Documentation
All relevant documentation can be found in decicated [Documentation][6] folder.

## Contributors
* Aleksa Brkić ([brksoman][7])
* Miljan Zarubica ([zarubica][8])
* Petar Đekanović ([petalex][9])

[1]: https://en.wikipedia.org/wiki/Sudoku
[2]: https://rti.etf.bg.ac.rs/rti/ir1pp2/index_si.html
[3]: https://en.wikipedia.org/wiki/Curses_(programming_library)#pcurses_and_PDCurses
[4]: http://www.mingw.org/wiki/Getting_Started
[5]: http://gnuwin32.sourceforge.net/packages/make.htm
[6]: https://github.com/petalex/sudoku/tree/master/docs
[7]: https://github.com/brksoman
[8]: https://github.com/zarubica
[9]: https://github.com/petalex
