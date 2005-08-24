
	new nosgmodel model
		sel model
		.setmodel tracks/switchR34R300.ive ""
	sel ..
	.setvisual model


	new nsecommands left
	left.addcmd2 model/anim setdval "switch" "0.999"
	left.addcmd3 str1 connecttrack next main prev
	left.execnow

	new nsecommands right
	right.addcmd2 model/anim setdval "switch" "0"
	right.addcmd3 right1 connecttrack prev main prev
#	right.execnow

	new nstateholder state
	state.setmomentary false
	state.addstate 0 left 0
	state.addstate 1 right 0

	new nsecommands changer
	changer.connecttomodelelement model Podsypka 0 0.0
	changer.addcmd2 state changestate 1 true
