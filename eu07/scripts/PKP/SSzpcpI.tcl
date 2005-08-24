	.settype "SEM_"

	new nosgmodel model
	model.setmodel PKP/SSzpcpI.ive ""
	.setvisual model


	new nsecommands s1
	s1.addcmd2 model setsequencemode  Green stop
	s1.addcmd2 model setsequencemode  UpperYellow stop
	s1.addcmd2 model setsequencevalue Red 1
	s1.addcmd2 model setsequencemode  LowerYellow stop
	s1.execnow

	new nsecommands s2
	s2.addcmd2 model setsequencevalue Green 1
	s2.addcmd2 model setsequencemode  UpperYellow stop
	s2.addcmd2 model setsequencemode  Red stop
	s2.addcmd2 model setsequencemode  LowerYellow stop
	
	new nsecommands s3
	s3.addcmd2 model setsequencemode  Green start
	s3.addcmd2 model setsequencemode  UpperYellow stop
	s3.addcmd2 model setsequencemode  Red stop
	s3.addcmd2 model setsequencemode  LowerYellow stop