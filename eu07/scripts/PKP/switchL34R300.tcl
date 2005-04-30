
	new nosgmodel model
		sel model
		.setmodel tracks/switchL34R300.ive ""
	sel ..
	.setvisual model


	new nsecommands left
	left.addcmd2 model/anim setdval "switch" "0.999"
	left.addcmd3 left1 connecttrack prev main prev
#	left.execnow

	new nsecommands right
	right.addcmd2 model/anim setdval "switch" "0"
	right.addcmd3 str1 connecttrack next main prev
	right.execnow

	new nstateholder state
	state.setmomentary false
	state.addstate 0 right 0
	state.addstate 1 left 0

	new nsecommands changer
	changer.connecttomodelelement model Podsypka 0 0.0
	changer.addcmd2 state changestate 1 true
