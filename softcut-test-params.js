outlets = 1

var msg = [
    ["rec", "1"],
    ["play", "1"],
    ["loop", "1"],
    ["rate", "1"],
    ["loop_start", "0"],
    ["loop_end", "2"],
    ["position", "0"]
];

function bang() {
	var sc = this.patcher.getnamed("softcut_0");
	for (i=0; i<msg.length; ++i) {
		//spost(msg[i]);
		// sc.message(msg[i][0], msg[i][1]);
		//outlet(0], msg[i]);
 	}
}
