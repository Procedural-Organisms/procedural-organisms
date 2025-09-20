thisProcess.openUDPPort(13933);
~host = NetAddr("localhost", 13933);

~host.sendMsg("/test", 42, "string", pi);

(
~osc = OSCFunc({|msg, time, addr, recvPort|
   [msg, time, addr, recvPort].postln;
},"/test")
);
