b = NetAddr.new("127.0.0.1", 11300);
b.sendMsg("/hello", "there";);

NetAddr.localAddr

thisProcess.openUDPPort(11301);

OSCdef.freeAll

n = NetAddr.new("127.0.0.1",11322);


o = OSCFunc({arg msg, time, addr, recvPort; [msg, time, addr, recvPort];},'/goodbye');


o.free


if(o > 1.0, o.postln)

OSCFunc()
)

(
OSCdef.new(\get, {
    arg msg, time, addr, port;
    [msg, time, addr, port].postln;
}, "/goodbye", nil, 11301);
)

OSCFunc.trace(false)
