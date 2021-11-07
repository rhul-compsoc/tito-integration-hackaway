# tito-integegration-hackaway
This is the [ti.to](ti.to) checkin and identity card system.

[![Main](https://github.com/rhul-compsoc/tito-integegration-hackaway/actions/workflows/main.yml/badge.svg)](https://github.com/rhul-compsoc/tito-integegration-hackaway/actions/workflows/main.yml) [![codecov](https://codecov.io/gh/rhul-compsoc/tito-integegration-hackaway/branch/main/graph/badge.svg?token=MKXK00CZMJ)](https://codecov.io/gh/rhul-compsoc/tito-integegration-hackaway)

enjoy the psuedo-javadoc comments

## Setup
This program is designed to run on linux, because the Hackaway setup has a bunch
of think centre computers that run Ubuntu.

| Environmental Variable | Usage |
|---|---|
| TITO_TOKEN | The live API token for the TiTo event. |
| TITO_ACCOUNT_SLUG | The account slug for the TiTo event, i.e: `royal-hackaway` |
| TITO_EVENT_SLUG | The event slug for the TiTo event, i.e: `v5` |

You should redirect `stderr` to a log file so that you do not get random (caught)
error messages in the middle of the GUI.
(This can be done with `tito-integration-hackaway 2> log`)

## Compiling
Use cmake to compile, the `DEBUG` to print extra information to stderr and, the
`TEST` flag to create compile the tests. 
With the tests you can run the 
`tito-integration-hackaway-tests` executable or `make coverage` to execute them.
