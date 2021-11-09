# tito-integegration-hackaway
This is the [ti.to](ti.to) checkin and identity card system.

[![Main](https://github.com/rhul-compsoc/tito-integration-hackaway/actions/workflows/main.yml/badge.svg)](https://github.com/rhul-compsoc/tito-integegration-hackaway/actions/workflows/main.yml) [![codecov](https://codecov.io/gh/rhul-compsoc/tito-integration-hackaway/branch/main/graph/badge.svg?token=MKXK00CZMJ)](https://codecov.io/gh/rhul-compsoc/tito-integration-hackaway)

Enjoy the psuedo-javadoc comments.

## Setup
This program is designed to run on linux, because the Hackaway setup has a bunch
of think centre computers that run Ubuntu.

| Environmental Variable | Usage |
|---|---|
| TITO_TOKEN | The live API token for the TiTo event. |
| TITO_ACCOUNT_SLUG | The account slug for the TiTo event, i.e: `royal-hackaway` |
| TITO_EVENT_SLUG | The event slug for the TiTo event, i.e: `v5` |
| TITO_CHECKIN_SLUG | The checkin slug (generated for web applications) |

You should redirect `stderr` to a log file so that you do not get random (caught)
error messages in the middle of the GUI.
(This can be done with `tito-integration-hackaway 2> log`)

## Usage
Run in a nice and, large terminal which suports colours. pipe stderr to a file
as debug information and errors are printed to it. Then use put the enviornemnt
variables are defined. An internet connection is required as well as, the working
directory has read and write permissions.

## Compiling
Use cmake to compile, the `DEBUG` to print extra information to stderr and, the
`TEST` flag to create compile the tests. 
With the tests you can run the 
`tito-integration-hackaway-tests` executable or `make coverage` to execute them.

## Tests
To run the tests it is recommended to not use the live tokens, and in fact use
another event which has no consequences if things go wrong.

## Future Maintainers
I am not sorry for using C++, I was told by someone that it is far more readable
than ANSI C so I took their advice and ++'ed my C.
