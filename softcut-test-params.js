outlets = 1

var msg = [
	"rec 1",
	"play 1",
	"loop 1",
	"rate 1",
	"loop_start 0",
	"loop_end 2",
	"position 0.0"
];

function bang() {
	for (i=0; i<msg.length; ++i) { 
	//messages.forEach(msg => { 
		//print(msg);
		outlet(0, msg[i]);
 	}
}