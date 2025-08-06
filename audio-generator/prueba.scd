Server.default.options.outDevice_("MacBook Pro Speakers")
s.boot

(play({
    PMOsc.ar(
        carfreq: [89.0, 194.0],
        modfreq: [356.0, 97.0],
        pmindex: [
            Saw.ar(freq: 0.291, mul: 1, add: 1),
            Saw.ar(freq: 0.267, mul: 1, add: 2)
        ],
        mul: [
            Saw.ar(freq: 0.291, mul: 0.002, add: 0.002),
            Saw.ar(freq: 0.267, mul: 0.0007, add: 0.0007)
        ]
    )
});
play({
    PMOsc.ar(
        carfreq: [158.0, 332.0],
        modfreq: [83.0, 79.0],
        pmindex: [
            SinOsc.ar(freq: 0.083, mul: 0.2, add: 0.1),
            SinOsc.ar(freq: 0.079, mul: 0.2, add: 0.1)
        ],
        mul: [
            SinOsc.ar(freq: 0.079, mul: 0.3, add: 0.05),
            SinOsc.ar(freq: 0.083, mul: 0.3, add: 0.05)
        ]
    )
}))
