softcut-msp
========

Max/MSP port of "softcut" audio processor (from monome's ['norns' project](https://github.com/monome/norns/)) originally written by [ezra buchla](https://github.com/catfact)

installing
========

head over to the [releases](https://github.com/AndrewShike/softcut-msp/releases) tab & download the latest thing, then drop `softcut~.mxo` from the .zip into your project folder, or add it to your [search path](https://docs.cycling74.com/max5/vignettes/core/search_path.html). now `softcut~` will be an available object !!

using
========

for now, `softcut~` only supports mono buffers, and the size of the buffer used by sofcut should be a number like `ceiling((2^n) / sample rate / 1000)`. so, `174763` (`n = 23` & `sample rate = 48k`) yields just short of 3 minutes, and `699051` (`n=25` and `sample rate = 48k`) gives ya a whopping 11.65 minutes. the first & only argument for the object sets the buffer to reference. a `set` message can change the referenced buffer.

reference the softcut [luadocs](https://monome.github.io/norns/doc/modules/softcut.html) for the list of softcut commands. here they take the form of a message contaning: "`command value`".

building
========

-   download Max SDK from cycling '74: [https://cycling74.com/downloads/sdk].
    this project was buit using the 8.0.3 version of the SDK.

-   clone this repo to `max-sdk-8.0.3/source/audio/softcut~`

-   build the xcode project.

-   this should create `max-sdk-8.0.3/externals/softcut~.mxo`; make sure your Max install can find it

-   run the test patch at `max-sdk-8.0.3/source/audio/softcut~/softcut-test.maxpat`

 
