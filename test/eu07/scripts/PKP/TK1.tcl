	.settype "SEM_"

	new nosgmodel model
		sel model
		.setmodel PKP/TK1.ive ""
	sel ..
	.setvisual model
	
	new nsecommands od1
	od1.addcmd2 model/anim setdval  anim 0
	od1.execnow

	new nsecommands od2
	od2.addcmd2 model/anim setdval  anim 0.999

	