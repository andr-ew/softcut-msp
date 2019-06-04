softcut-msp
========

Max/MSP port of "softcut" audio processor (from monome's ['norns' project](https://github.com/monome/norns/))

building
========

-   download Max SDK from cycling '74: [https://cycling74.com/downloads/sdk].
    this project was buit using the 8.0.3 version of the SDK.

-   clone this repo to `max-sdk-8.0.3/source/audio/softcut~`

-   build the xcode project.

-   this should create `max-sdk-8.0.3/externals/softcut~.mxo`; make sure your Max install can find it

-   run the test patch at `max-sdk-8.0.3/source/audio/softcut~/softcut-test.maxpat`

 
