
new nvehicle $name
	sel $name

	.settype real

	new ncamerahandle cabA
	cabA.setoffset  0.7  5.8 3
	new ncamerahandle cabB
	cabB.setoffset -0.7 -5.8 3

	new ncamerahandle ext1
	ext1.setoffset 0  0 6
	ext1.setr 20

	new ncamerahandle ext2
	ext2.setoffset 5  0 1.8
	


	new nosgmodel visual
	visual.setmodel dynamic/pkp/eu07/EU07-303E-m.ive "dynamic/pkp/eu07/399"

	.setvisual visual

	sel audio
		new nsimsound motor1
			sel motor1
			.setfile "sounds/motor-et22.wav"
			.setlooping true
			.setminmaxdist 30 200
			.setambient true
			new nsimipol fr
			fr.connect setfrequency
			fr.beginkeys 2 1
			fr.setkey1f 0 0 0
			fr.setkey1f 1 500 10
			fr.endkeys
			fr.setchannel "motor_fr"
			fr.setreptype oneshot
			new nsimipol vol
			vol.connect setvolume
			vol.beginkeys 4 1
#			vol.setkey1f 0 0 0.8
#			vol.setkey1f 1 50 0.8
#			vol.setkey1f 2 90 0.2
#			vol.setkey1f 3 1200 0.2
			vol.setkey1f 0 0 0.9
			vol.setkey1f 1 100 0.9
			vol.setkey1f 2 120 0.815
			vol.setkey1f 3 1200 0.8
			vol.endkeys
			vol.setchannel "motor_vol"
			vol.setreptype oneshot
			.start
		sel ..
		new nsimsound motor2
			sel motor2
			.setfile "sounds/motor-eu07.wav"
			.setlooping true
			.setminmaxdist 30 200
			.setambient true
			new nsimipol fr
			fr.connect setfrequency
			fr.beginkeys 2 1
			fr.setkey1f 0 0 0
			fr.setkey1f 1 600 10
			fr.endkeys
			fr.setchannel "motor_fr"
			fr.setreptype oneshot
			new nsimipol vol
			vol.connect setvolume
			vol.beginkeys 4 1
#			vol.setkey1f 0 0 0.2
#			vol.setkey1f 1 50 0.2
#			vol.setkey1f 2 90 0.8
#			vol.setkey1f 3 1200 0.8
			vol.setkey1f 0 0 0.3
			vol.setkey1f 1 60 0.5
			vol.setkey1f 2 100 0.855
			vol.setkey1f 3 1200 0.9
 			vol.endkeys
			vol.setchannel "motor_vol"
			vol.setreptype oneshot
			.start
		sel ..


		new nsimsound wheel04
			sel wheel04
			.setfile "sounds/wheel-medium.wav"
			.settriggerchannel "wheel04snd"
			.setoffset 0 5.8 0
		sel ..
		new nsimsound wheel03
			sel wheel03
			.setfile "sounds/wheel-medium.wav"
			.settriggerchannel "wheel03snd"
			.setoffset 0 2.7 0
		sel ..

		new nsimsound wheel02
			sel wheel02
			.setfile "sounds/wheel-medium.wav"
			.settriggerchannel "wheel02snd"
			.setoffset 0 -2.7 0
		sel ..
		new nsimsound wheel01
			sel wheel01
			.setfile "sounds/wheel-medium.wav"
			.settriggerchannel "wheel01snd"
			.setoffset 0 -5.8 0
		sel ..


#		new nsimsound buzzer2
#			sel buzzer2
#			.setfile "sounds/bzyczek.wav"
#			.setambient true
#			.setlooping true
#			.start
#		sel ..
	sel ..

	.setnumaxles 4
	.setstdparams 8.55 16.260 2
	.initpantograph 1 0 4.2486  3.3186  0.8
	.initpantograph 2 0 4.2486 -3.3186  0.8

	# PARAMETRY FIZYKI
	# podstawowe parametry: kategoria, masa [kg], Vmax [km/h], Pmax silnika [kW], Fmax silnika [kN] 
	.basicparam train 83500 125 500 175
	# wymiary: dlugosc, wysokosc, szerokosc, Cx
	.sizeparam 16.260 4.343 2.903 0.3 

	# urz. zd-sprz: przod/tyl, zderzak/sprzeg, typ, k [kN/m], d [m], Fmax [kN], tlum.dyn. [kN*s/m], tlum.stat. []
	.interface 0 buffer dual 9200 0.15 820 100 0.58 
	.interface 0 coupler screw 4800 0.15 780 100 0.58
	.interface 1 buffer dual 9200 0.15 820 100 0.58 
	.interface 1 coupler screw 4800 0.15 780 100 0.58

	# nastawniki: masterctrl, scndctrl, mainbrake, helperbrake
	.ctrlparam 42 6 6 10 

	# kola, dane ogolne: rozstaw [m], uklad, odleglosc osi[m], odleglosc czopow [m], min R [m], typ lozyska, sila docisku klocka
	.wheelparam 1.435 "Bo'Bo'" 3.05 8.55 80 "Roll" 70000

	# kola: odleglosc, srednica, moment bezwladnosci, przekladnia, stopien obciazenia, ilosc klockow na os, kontrola predkosci 
	.axleparam 1 [expr (-3.05-8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 1
	.axleparam 2 [expr ( 3.05-8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0
	.axleparam 3 [expr (-3.05+8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0
	.axleparam 4 [expr ( 3.05+8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0

