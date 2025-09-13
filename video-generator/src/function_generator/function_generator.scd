s.makeWindow()
play({SinOsc.ar(LFNoise0.kr(12!2, 600, 1000), 0.0, 0.1)})
play({RLPF.ar(Dust.ar([12, 15]), LFNoise1.ar(1/[3, 4], 1500, 1600), 0.02)})

(
play({
   var sines = 5, speed = 6;
   Mix.fill(sines,
        {arg x;
             Pan2.ar(
                  SinOsc.ar(x+1*100,
                        mul: max(0,
                              LFNoise1.kr(speed) +
                              Line.kr(1, −1, 30)
                        )
), rand2(1.0))})/sines})
)

{rand(20)}!10

2!10
