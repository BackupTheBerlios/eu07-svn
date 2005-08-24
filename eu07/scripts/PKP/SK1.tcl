	.settype "SEM_"

	new nosgmodel model
		sel model
		.setmodel PKP/SK1.ive ""
	sel ..
	.setvisual model
	
	new nsecommands sr1
	sr1.addcmd2 model/anim setdval  ramie_gora 0
	sr1.execnow

	new nsecommands sr2
	sr2.addcmd2 model/anim setdval  ramie_gora 0.999

	