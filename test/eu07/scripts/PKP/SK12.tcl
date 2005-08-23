	.settype "SEM_"

	new nosgmodel model
		sel model
		.setmodel PKP/SK12.ive ""
	sel ..
	.setvisual model
	
	new nsecommands sr1
	sr1.addcmd2 model/anim setdval  ramie_gora 0
	sr1.addcmd2 model/anim setdval  ramie_dol 0
	sr1.execnow

	new nsecommands sr2
	sr2.addcmd2 model/anim setdval  ramie_gora 0.999
	sr2.addcmd2 model/anim setdval  ramie_dol 0

	new nsecommands sr3
	sr3.addcmd2 model/anim setdval  ramie_gora 0.999
	sr3.addcmd2 model/anim setdval  ramie_dol 0.999

	