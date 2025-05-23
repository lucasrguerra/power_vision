<Qucs Schematic 24.4.1>
<Properties>
  <View=-482,-149,2333,593,1.4641,977,0>
  <Grid=10,10,1>
  <DataSet=simulation.dat>
  <DataDisplay=simulation.dpl>
  <OpenDisplay=0>
  <Script=simulation.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Título>
  <FrameText1=Desenhado por:>
  <FrameText2=Data:>
  <FrameText3=Revisão:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Vac V2 1 400 340 -26 18 0 0 "311.08 V" 1 "60 Hz" 0 "120" 0 "0" 0 "0" 0 "0" 0>
  <R R2 1 490 340 -26 15 0 0 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <MUT Tr2 1 550 370 -29 38 0 0 "1 mH" 0 "1 mH" 0 "0.9" 0>
  <Vac V1 1 400 200 -26 18 0 0 "311.08 V" 1 "60 Hz" 0 "0" 0 "0" 0 "0" 0 "0" 0>
  <R R1 1 490 200 -26 15 0 0 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <MUT Tr1 1 550 230 -29 38 0 0 "1 mH" 0 "1 mH" 0 "0.9" 0>
  <Vac V3 1 400 480 -26 18 0 0 "311.08 V" 1 "60 Hz" 0 "240" 0 "0" 0 "0" 0 "0" 0>
  <R R3 1 490 480 -26 15 0 0 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <MUT Tr3 1 550 510 -29 38 0 0 "1 mH" 0 "1 mH" 0 "0.9" 0>
  <R R8 1 430 -40 15 -26 0 1 "10 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R7 1 430 40 15 -26 0 1 "1 MOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R9 1 690 -10 -26 -47 0 2 "1 MOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe Pr1 1 760 -30 28 -31 0 0>
  <Vdc V4 1 830 -40 18 -26 0 1 "3.3 V" 1>
  <R R10 1 930 -70 -26 -47 0 2 "10 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R11 1 1010 -70 -26 -47 0 2 "10 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <OpAmp OP4 1 950 50 -75 -26 0 3 "1e6" 1 "15 V" 0>
  <R R12 1 950 120 15 -26 0 1 "50 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <OpAmp OP1 1 670 220 -26 42 0 0 "1e6" 1 "15 V" 0>
  <R R4 1 720 300 -26 -47 0 2 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <OpAmp OP2 1 670 360 -26 42 0 0 "1e6" 1 "15 V" 0>
  <R R5 1 720 440 -26 -47 0 2 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <OpAmp OP3 1 670 500 -26 42 0 0 "1e6" 1 "15 V" 0>
  <R R6 1 720 580 -26 -47 0 2 "205 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe Pr2 1 800 200 28 -31 0 0>
  <VProbe Pr3 1 800 340 28 -31 0 0>
  <VProbe Pr4 1 800 480 28 -31 0 0>
  <GND * 1 830 -10 0 0 0 0>
  <GND * 1 860 500 0 0 0 0>
  <.DC DC1 1 610 50 0 40 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <Lib X2 1 570 -40 70 -16 0 0 "Optocoupler" 0 "4N25" 0>
</Components>
<Wires>
  <360 340 370 340 "" 0 0 0 "">
  <430 340 460 340 "" 0 0 0 "">
  <360 340 360 400 "" 0 0 0 "">
  <360 400 520 400 "" 0 0 0 "">
  <360 200 360 260 "" 0 0 0 "">
  <360 200 370 200 "" 0 0 0 "">
  <430 200 460 200 "" 0 0 0 "">
  <360 260 360 340 "" 0 0 0 "">
  <360 260 520 260 "" 0 0 0 "">
  <360 400 360 480 "" 0 0 0 "">
  <360 480 370 480 "" 0 0 0 "">
  <430 480 460 480 "" 0 0 0 "">
  <360 480 360 540 "" 0 0 0 "">
  <360 540 520 540 "" 0 0 0 "">
  <430 -10 430 0 "" 0 0 0 "">
  <430 0 430 10 "" 0 0 0 "">
  <430 70 430 200 "" 0 0 0 "">
  <360 -70 430 -70 "" 0 0 0 "">
  <360 -70 360 200 "" 0 0 0 "">
  <720 -10 750 -10 "" 0 0 0 "">
  <770 -10 830 -10 "" 0 0 0 "">
  <830 -10 1040 -10 "" 0 0 0 "">
  <1040 -70 1040 -10 "" 0 0 0 "">
  <830 -70 900 -70 "" 0 0 0 "">
  <960 -70 970 -70 "" 0 0 0 "">
  <970 -70 980 -70 "" 0 0 0 "">
  <970 -70 970 20 "" 0 0 0 "">
  <930 10 930 20 "" 0 0 0 "">
  <870 10 930 10 "" 0 0 0 "">
  <870 10 870 90 "" 0 0 0 "">
  <870 90 950 90 "" 0 0 0 "">
  <580 260 630 260 "" 0 0 0 "">
  <580 200 640 200 "" 0 0 0 "">
  <630 240 640 240 "" 0 0 0 "">
  <630 300 690 300 "" 0 0 0 "">
  <750 300 770 300 "" 0 0 0 "">
  <770 220 770 300 "" 0 0 0 "">
  <710 220 770 220 "" 0 0 0 "">
  <630 240 630 260 "" 0 0 0 "">
  <630 260 630 300 "" 0 0 0 "">
  <580 540 630 540 "" 0 0 0 "">
  <580 400 630 400 "" 0 0 0 "">
  <580 480 640 480 "" 0 0 0 "">
  <580 340 640 340 "" 0 0 0 "">
  <630 380 640 380 "" 0 0 0 "">
  <630 380 630 400 "" 0 0 0 "">
  <630 440 690 440 "" 0 0 0 "">
  <750 440 770 440 "" 0 0 0 "">
  <770 360 770 440 "" 0 0 0 "">
  <710 360 770 360 "" 0 0 0 "">
  <630 400 630 440 "" 0 0 0 "">
  <630 520 640 520 "" 0 0 0 "">
  <630 580 690 580 "" 0 0 0 "">
  <750 580 770 580 "" 0 0 0 "">
  <770 500 770 580 "" 0 0 0 "">
  <710 500 770 500 "" 0 0 0 "">
  <630 520 630 540 "" 0 0 0 "">
  <630 540 630 580 "" 0 0 0 "">
  <580 460 580 480 "" 0 0 0 "">
  <580 460 610 460 "" 0 0 0 "">
  <610 150 610 180 "" 0 0 0 "">
  <610 150 950 150 "" 0 0 0 "">
  <580 320 580 340 "" 0 0 0 "">
  <610 320 610 460 "" 0 0 0 "">
  <580 320 610 320 "" 0 0 0 "">
  <580 180 580 200 "" 0 0 0 "">
  <610 180 610 320 "" 0 0 0 "">
  <580 180 610 180 "" 0 0 0 "">
  <810 220 860 220 "" 0 0 0 "">
  <860 220 860 360 "" 0 0 0 "">
  <810 360 860 360 "" 0 0 0 "">
  <860 360 860 500 "" 0 0 0 "">
  <810 500 860 500 "" 0 0 0 "">
  <770 500 790 500 "" 0 0 0 "">
  <770 360 790 360 "" 0 0 0 "">
  <770 220 790 220 "" 0 0 0 "">
  <600 -10 660 -10 "" 0 0 0 "">
  <600 -70 830 -70 "" 0 0 0 "">
  <430 0 540 0 "" 0 0 0 "">
  <540 -10 540 0 "" 0 0 0 "">
  <430 -70 540 -70 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
