~host = NetAddr("localhost", NetAddr.langPort);

~host.sendMsg("/leftPerc", 42, "string", pi);

(
~osc = OSCFunc({|msg, time, addr, recvPort|
   [msg, time, addr, recvPort].postln;
},"/leftPerc", ~host)
);
