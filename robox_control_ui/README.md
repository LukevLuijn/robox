# Robox application

This page contains the software application for the ROBOX project. The software application uses the libraries wxWidgets & boost.

The messaging protocol used in this project can be found [here](https://github.com/LukevLuijn/robox_docs/blob/main/protocol/protocol_description.pdf).

The Interface description can be found [here]()



For the full documentation go to [this page](https://github.com/LukevLuijn/robox_docs). All documentation is written in dutch.

## Installation instructions

```bash
# From the root of the project folder.

$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./robox
```

## Class diagrams

||
|:---:|
|<img width="10000" src="diagrams/DCD_software.svg"  alt=""/>|
| **Diagram 1** - *Design class diagram*|
|<img width="10000" src="diagrams/CD_base.svg"  alt=""/>|
| **Diagram 1** - *Package Base*|
|<img width="10000" src="diagrams/CD_driver.svg"  alt=""/>|
| **Diagram 1** - *Package Driver*|
|<img width="10000" src="diagrams/CD_frame.svg"  alt=""/>|
| **Diagram 1** - *Package Frame*|
|<img width="10000" src="diagrams/CD_logger.svg"  alt=""/>|
| **Diagram 1** - *Package Logger*|
|<img width="10000" src="diagrams/CD_utils.svg"  alt=""/>|
| **Diagram 1** - *Package Utils*|
|<img width="10000" src="diagrams/CD_widgets.svg"  alt=""/>|
| **Diagram 1** - *Package Widgets*|