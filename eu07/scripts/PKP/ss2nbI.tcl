	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/ss2nbI.ive ""
	.setvisual model


	new nsecommands ms1
	ms1.addcmd2 model setsequencevalue  Blue 1
	ms1.addcmd2 model setsequencemode White stop
      ms1.execnow

      new nsecommands ms2
	ms2.addcmd2 model setsequencemode Blue stop
	ms2.addcmd2 model setsequencevalue  White 1
	