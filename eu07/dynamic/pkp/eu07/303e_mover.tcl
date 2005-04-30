
new nmover $name
	sel $name

	.openchk "303E"

	.setnumpantographs 2
	.initpantograph 1 0 4.2486  3.3186  0.8
	.initpantograph 2 0 4.2486 -3.3186  0.8


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
			fr.addkey1f 0 0
			fr.addkey1f 500 10
			fr.endkeys
			fr.setchannel "motor_fr"
			fr.setreptype oneshot
			new nsimipol vol1
			vol1.connect setvolume1
			vol1.beginkeys 4 1
			vol1.addkey1f 0 0.9
			vol1.addkey1f 100 0.9
			vol1.addkey1f 120 0.815
			vol1.addkey1f 1200 0.8
			vol1.endkeys
			vol1.setchannel "motor_vol"
			vol1.setreptype oneshot
			new nsimipol vol2
			vol2.connect setvolume2
			vol2.beginkeys 2 1
			vol2.addkey1f 0 0
			vol2.addkey1f 6000000 1
			vol2.endkeys
			vol2.setchannel "EnginePower"
			vol2.setreptype oneshot
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
			fr.addkey1f 0 0
			fr.addkey1f 600 10
			fr.endkeys
			fr.setchannel "motor_fr"
			fr.setreptype oneshot
			new nsimipol vol1
			vol1.connect setvolume1
			vol1.beginkeys 4 1
			vol1.addkey1f 0 0.3
			vol1.addkey1f 60 0.5
			vol1.addkey1f 100 0.855
			vol1.addkey1f 1200 0.9
 			vol1.endkeys
			vol1.setchannel "motor_vol"
			vol1.setreptype oneshot
			new nsimipol vol2
			vol2.connect setvolume2
			vol2.beginkeys 2 1
			vol2.addkey1f 0 0
			vol2.addkey1f 6000000 1
			vol2.endkeys
			vol2.setchannel "EnginePower"
			vol2.setreptype oneshot
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

