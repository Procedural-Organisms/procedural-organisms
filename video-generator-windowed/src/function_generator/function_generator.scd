~host = NetAddr("localhost", NetAddr.langPort);

~host.sendMsg("/test", 42, "string", pi);

~host.sendBundle(
    0.5,
    ["/test", 200, "bbb", pi],
    ["/test", 300]
)

~host.sendBundle(10,
  ["/test", 42, "string", pi],
  ["/test", 183]);

~osc = OSCFunc(
    {
        |msg, time, addr, recvPort|
        [msg, time, addr, recvPort].postln;
    },"/test", ~host
)
