source scripts/utilities.tcl

/sys/servers/gfx.pushdatapath "textures"
/sys/servers/gfx.pushdatapath "models"
/sys/servers/gfx.pushdatapath "models/signals"

#/sys/servers/gfx.hidecursor
/sys/servers/gfx.load "scenery/test/main.ive"
/world.open "scenery/test/main.scn"


#addMover eu07-424 PKP/EU07/303e_mover_ctrl.tcl abc
#addMoverTo bd1 PKP/BD/111a_mover.tcl abc eu07-424
#addMover eu07-424 PKP/BD/111a_mover.tcl abc 
addVehicle eu07-424 PKP/EU07/303e_ctrl.tcl abc

#addVehicle ty2-test PKP/ty2/ty2_ctrl.tcl abc
#addVehicleTo ty2-test2 PKP/ty2/ty2beczka.tcl abc ty2-test
#for {set i 0} {$i<20} {incr i} {
#	addVehicleTo eaos$i PKP/eaos/eaos.tcl abc ty2-test
#}
#addVehicleTo eaos2 PKP/eaos/eaos.tcl abc ty2-test

#addTestMover

sel /world/dynamics/eu07-424

/world.setsimtime 11 30

	sel audio
		new nsimsound odjazd
			sel odjazd
			.setfile "sounds/3342odjazd.wav"
			.setambient true
		sel ..
	sel ..

	new nsecommands odjazd
	odjazd.addcmd audio/odjazd start

	new nsimpletimetable tt
	tt.addentry     elana  11 31  11 32  15  odjazd
	tt.addentry  grebocin  11 35  11 36  15  odjazd
