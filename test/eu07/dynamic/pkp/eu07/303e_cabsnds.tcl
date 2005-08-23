
	sel audio
        new nsimsound hiss
            sel hiss
            .setfile "sounds/hiss.wav"
            .setambient true
			.setvolume 0

			new nsimipol fr1
			fr1.connect setfrequency
			fr1.beginkeys 2 1
			fr1.addkey1f 0 0.8
			fr1.addkey1f 0.01 0.8
			fr1.endkeys
			fr1.setchannel "HissRelease"
			fr1.setreptype oneshot
			new nsimipol fr2
			fr2.connect setfrequency
			fr2.beginkeys 2 1
			fr2.addkey1f 0 1
			fr2.addkey1f 0.01 1
			fr2.endkeys
			fr2.setchannel "HissBrake"
			fr2.setreptype oneshot

			new nsimipol vol1
			vol1.connect setvolume
			vol1.beginkeys 2 1
			vol1.addkey1f 0 0
			vol1.addkey1f 0.01 2
			vol1.endkeys
			vol1.setchannel "HissRelease"
			vol1.setreptype oneshot
			new nsimipol vol2
			vol2.connect setvolume
			vol2.beginkeys 2 1
			vol2.addkey1f 0 0
			vol2.addkey1f 0.01 2
			vol2.endkeys
			vol2.setchannel "HissBrake"
			vol2.setreptype oneshot

			.setlooping true
            .start
        sel ..
		new nsimsound sw
			sel sw
			.setfile "sounds/switch.wav"
			.setambient true
		sel ..
        new nsimsound converter
            sel converter
            .setfile "sounds/converter-eu07.wav"
            .setambient true
			.setvolume 1
			.setlooping true
			.setstartoffset 2763
			.setstopoffset  7286
#			.setstopoffset 24588
#            .start
        sel ..
        new nsimsound compressor
            sel compressor
            .setfile "sounds/compressor.wav"
            .setambient true
			.setvolume 1
			.setlooping true
			.setstartoffset 504
			.setstopoffset 7000
#			.setstopoffset 4327
#            .start
        sel ..

		new nsimsound buzzer
			sel buzzer
			.setfile "sounds/bzyczek.wav"
			.setambient true
			.setlooping true
#			.stop
		sel ..
		new nsimsound hasler
			sel hasler
			.setfile "sounds/hasler.wav"
			.setambient true
			.setlooping true
		sel ..
		new nsimsound rvent
			sel rvent
			.setfile "sounds/rvent-eu07.wav"
			.setambient true
			.setlooping true
			new nsimipol fr
			fr.connect setfrequency
			fr.beginkeys 3 1
			fr.addkey1f 0 0.5
			fr.addkey1f 25 1
			fr.addkey1f 100 1
			fr.endkeys
			fr.setchannel "Vent"
			fr.setreptype oneshot
			new nsimipol vol1
			vol1.connect setvolume
			vol1.beginkeys 3 1
			vol1.addkey1f 0 0
			vol1.addkey1f 10 1
			vol1.addkey1f 100 1
			vol1.endkeys
			vol1.setchannel "Vent"
			vol1.setreptype oneshot
			.start
		sel ..

		
	sel ..

	compressor_on.addcmd audio/compressor start
	compressor_off.addcmd audio/compressor stop

