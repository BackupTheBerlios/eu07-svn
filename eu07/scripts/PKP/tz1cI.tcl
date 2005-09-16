	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/tz1cI.ive ""
	.setvisual model


	new nsecommands s1
    	s1.addcmd2 model setsequencevalue  Red 1
      s1.execnow

	